# 上位机与单片机通信代码提取（基于STM32 + CUBEAI手写识别案例）

## 一、核心通信配置（底层初始化）

### 1. USART1 初始化配置（串口通信核心）

```C

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  /* USER CODE BEGIN USART1_Init 0 */
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */
  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;  // 波特率115200（需与上位机一致）
  huart1.Init.WordLength = UART_WORDLENGTH_8B;  // 8位数据位
  huart1.Init.StopBits = UART_STOPBITS_1;       // 1位停止位
  huart1.Init.Parity = UART_PARITY_NONE;        // 无校验位
  huart1.Init.Mode = UART_MODE_TX_RX;           // 收发模式
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;  // 无硬件流控
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;  // 16倍过采样
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  /* USER CODE END USART1_Init 2 */
}
```

### 2. 通信相关全局变量定义

```C

/* 串口通信缓冲区配置 */
#define UART_BUFF_LEN 1024          // 串口接收缓冲区总长度
#define ONE_FRAME_LEN 1+784+2       // 单帧数据长度：1（帧头）+784（28*28手写数据）+2（帧尾）
uint16_t uart_rx_length = 0;        // 已接收数据长度计数
uint8_t uart_rx_byte = 0;           // 单次接收字节缓存
uint8_t uart_rx_buffer[UART_BUFF_LEN];  // 接收缓冲区（存储上位机发送的完整数据）
volatile uint8_t goRunning = 0;     // 接收完成标志（0：未完成，1：已完成一帧接收）

char message[]="hello";             // 测试发送数据（可选）
```

## 二、数据接收代码（上位机→单片机）

### 1. 中断接收回调函数（核心接收逻辑）

通过串口中断方式接收上位机发送的手写数字数据，触发条件为接收完一帧数据（以`\n`为帧尾标志）。

```C

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  if(goRunning == 0)  // 未开始接收或上一帧已处理完成
  {
    if (uart_rx_length < UART_BUFF_LEN)  // 缓冲区未溢出
    {
      uart_rx_buffer[uart_rx_length] = uart_rx_byte;  // 存储接收字节
      uart_rx_length++;

      if (uart_rx_byte == '\n')  // 检测到帧尾（换行符），接收完成
      {
        goRunning = 1;  // 置位接收完成标志，通知主循环处理数据
      }
    }
    else
    {
      // 缓冲区溢出，重置计数
      uart_rx_length = 0;
    }
  }
  // 重新开启中断接收（单次中断仅接收1字节，需循环开启）
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_rx_byte, 1);
}
```

### 2. 接收初始化（主函数中启动）

```C

int main(void)
{
  /* 省略其他初始化代码... */

  /* USER CODE BEGIN 2 */
  AI_Init();  // 神经网络初始化（非通信相关，仅为代码完整性）
  memset(uart_rx_buffer, 0, 784);  // 清空接收缓冲区
  // 开启串口中断接收（首次启动）
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_rx_byte, 1);
  /* USER CODE END 2 */

  /* 省略主循环... */
}
```

## 三、数据发送代码（单片机→上位机）

### 1. 串口发送函数（字符串形式）

用于将识别结果（数字概率、最终识别值）回传给上位机。

```C

/* 串口发送字符串函数 */
void Uart_send(char * str)
{
  // 阻塞式发送：发送str字符串，超时时间0xFFFF（足够长）
  HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), 0xffff);
}
```

### 2. 识别结果发送逻辑（集成在AI推理后）

```C

static void AI_Run(float *pIn, float *pOut)
{
  char logStr[100];  // 发送数据缓存
  int count = 0;     // 识别结果（概率最大的数字）
  float max = 0;     // 最大概率值
  ai_i32 batch;
  ai_error err;

  /* 省略神经网络推理核心代码... */

  // 发送每个数字的识别概率（0-9）
  for (uint32_t i = 0; i < AI_NETWORK_OUT_1_SIZE; i++) {
    sprintf(logStr, "%d  %8.6f\r\n", i, aiOutData[i]);  // 格式化：数字+概率
    Uart_send(logStr);  // 发送到上位机
    // 查找概率最大值对应的数字
    if (max < aiOutData[i])
    {
      count = i;
      max = aiOutData[i];
    }
  }
  // 发送最终识别结果
  sprintf(logStr, "current number is %d\r\n", count);
  Uart_send(logStr);
}
```

## 四、数据处理与通信流程衔接（主循环）

```C

/* 无限循环：处理接收数据→AI推理→发送结果 */
while (1)
{
  /* USER CODE END WHILE */
  /* USER CODE BEGIN 3 */
  Uart_send(message);  // 可选：发送测试信息给上位机
  if (goRunning > 0)  // 检测到一帧数据接收完成
  {
    if (uart_rx_length == ONE_FRAME_LEN)  // 验证数据长度是否正确
    {
      // 数据格式转换：上位机8位数据→单片机32位浮点数（适配模型输入）
      PictureCharArrayToFloat(uart_rx_buffer + 1, aiInData, 28*28);
      // AI推理：输入手写数据，输出识别结果
      AI_Run(aiInData, aiOutData);
    }
    // 重置接收状态，准备下一帧接收
    memset(uart_rx_buffer, 0, 784);  // 清空缓冲区
    goRunning = 0;                   // 重置接收完成标志
    uart_rx_length = 0;              // 重置接收长度计数
  }
}
```

## 五、数据格式转换函数（通信与模型适配）

上位机发送的是8位字节数据，单片机模型需要32位浮点数输入，因此需进行格式转换：

```C

/* 8位字节数组→32位浮点数数组（适配模型输入） */
void PictureCharArrayToFloat(uint8_t *srcBuf, float *dstBuf, int len)
{
  for (int i = 0; i < len; i++)
  {
    dstBuf[i] = srcBuf[i];  // 直接转换（若上位机数据有缩放，需在此处调整）
    // 可选：如需二值化处理，可改为 dstBuf[i] = (srcBuf[i] == 1) ? 0 : 1;
  }
}
```

## 六、通信关键说明

1. **通信参数**：波特率115200、8N1（8位数据位+1位停止位+无校验），需与上位机严格一致。

2. **数据帧格式**：单帧长度=787字节（1字节帧头+784字节手写图像数据+2字节帧尾），帧尾以`\n`（换行符）作为接收完成标志。

3. **传输方向**：

    - 上行：上位机→单片机（手写数字图像数据，28×28像素，共784字节）；

    - 下行：单片机→上位机（0-9每个数字的识别概率+最终识别结果）。

4. **中断机制**：采用串口中断接收（`HAL_UART_Receive_IT`），避免阻塞主循环，提高实时性。
