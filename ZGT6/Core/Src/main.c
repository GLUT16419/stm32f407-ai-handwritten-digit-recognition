/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "app_x-cube-ai.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


#include <string.h>
#include <stdio.h>
#include <math.h>
#include "network.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* 串口通信全局变量定义 */
uint16_t uart_rx_length = 0;        // 已接收数据长度计数
uint8_t uart_rx_byte = 0;           // 单次接收字节缓存
uint8_t uart_rx_buffer[UART_BUFF_LEN];  // 接收缓冲区（存储上位机发送的完整数据）
volatile uint8_t goRunning = 0;     // 接收完成标志（0：未完成，1：已完成一帧接收）

/* AI模型输入输出缓冲区 */
float aiInData[28*28];              // AI模型输入数据（28*28=784个float值）
float aiOutData[10];                 // AI模型输出数据（0-9概率）

/* AI模型相关变量 */
extern ai_handle network;            // 从app_x-cube-ai.c导入网络句柄

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* 8位字节数组→32位浮点数数组（适配模型输入） */
void PictureCharArrayToFloat(uint8_t *srcBuf, float *dstBuf, int len)
{
  for (int i = 0; i < len; i++)
  {
    dstBuf[i] = srcBuf[i];  // 直接转换（若上位机数据有缩放，需在此处调整）
    // 可选：如需归一化处理，可改为 dstBuf[i] = srcBuf[i] / 255.0f;
  }
}

/* 串口发送字符串函数 */
void Uart_send(char * str)
{
  if (str != NULL) {
    uint16_t len = strlen(str);
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart1, (uint8_t *)str, len, 0xffff);
    if (status != HAL_OK) {
      // 发送失败时的处理
      char errMsg[50];
      sprintf(errMsg, "UART send failed: %d\r\n", status);
      // 尝试再次发送错误信息
      HAL_UART_Transmit(&huart1, (uint8_t *)errMsg, strlen(errMsg), 0xffff);
    }
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* 系统启动开始 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* 启动串口接收中断 */
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_rx_byte, 1);
  printf("USART1 initialized\r\n");
  MX_X_CUBE_AI_Init();
  /* USER CODE BEGIN 2 */

  printf("System startup completed\r\n");
  printf("Ready to receive data\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

  MX_X_CUBE_AI_Process();
    /* USER CODE BEGIN 3 */
    if (goRunning > 0)  // 检测到一帧数据接收完成
    {
      if (uart_rx_length == ONE_FRAME_LEN)  // 验证数据长度是否正确
      {
        // 检查AI网络是否初始化成功
        if (network != AI_HANDLE_NULL) {
          // 数据格式转换：上位机8位数据→单片机32位浮点数（适配模型输入）
          PictureCharArrayToFloat(uart_rx_buffer + 1, aiInData, 28*28);
          
          // 准备AI模型输入数据
          ai_buffer* input = ai_network_inputs_get(network, NULL);
          if (input) {
            // 将aiInData复制到模型输入缓冲区
            memcpy(input[0].data, aiInData, sizeof(aiInData));
            
            // 准备AI模型输出缓冲区
            ai_buffer* output = ai_network_outputs_get(network, NULL);
            if (output) {
              // AI推理：输入手写数据，输出识别结果
              ai_i32 batch = ai_network_run(network, input, output);
              if (batch == 1) {
                // 从模型输出获取识别结果
                memcpy(aiOutData, output[0].data, sizeof(aiOutData));
                
                // 计算指数值和百分比
                int count = 0;
                float max = aiOutData[0];
                float exp_values[10];
                float exp_sum = 0;
                char logStr[100];
                
                // 发送自然常数e的值
                Uart_send("Natural constant e ~= 2.71828\r\n\r\n");
                
                // 发送表头
                Uart_send("Num  Score      Exp       Prob(%)\r\n");
                
                // 计算指数值和指数和
                for (uint32_t i = 0; i < 10; i++) {
                  exp_values[i] = exp(aiOutData[i]);
                  exp_sum += exp_values[i];
                }
                
                // 发送每个数字的详细信息
                for (uint32_t i = 0; i < 10; i++) {
                  float probability = (exp_values[i] / exp_sum) * 100;
                  sprintf(logStr, "%d    %8.6f   %.3f     %.2f\r\n", i, aiOutData[i], exp_values[i], probability);
                  Uart_send(logStr);
                  // 查找概率最大值对应的数字
                  if (max < aiOutData[i]) {
                    count = i;
                    max = aiOutData[i];
                  }
                }
                
                // 发送分隔线
                Uart_send("-----------------\r\n");
                
                // 发送指数和
                sprintf(logStr, "Exponential sum: %.3f\r\n", exp_sum);
                Uart_send(logStr);
                
                // 发送分隔线
                Uart_send("-----------------\r\n");
                
                // 计算最终结果的百分比
                float final_probability = (exp(max) / exp_sum) * 100;
                
                // 发送最终识别结果
                sprintf(logStr, "current number is %d\r\n", count);
                Uart_send(logStr);
                
                // 发送概率值
                sprintf(logStr, "probability: %8.6f (%.2f%%)\r\n", max, final_probability);
                Uart_send(logStr);
              } else {
                Uart_send("AI inference failed\r\n");
              }
            } else {
              Uart_send("Failed to get AI network outputs\r\n");
            }
          } else {
            Uart_send("Failed to get AI network inputs\r\n");
          }
        } else {
          Uart_send("AI network not initialized\r\n");
        }
      } else {
        char errorMsg[100];
        sprintf(errorMsg, "Invalid data length: %d (expected: %d)\r\n", uart_rx_length, ONE_FRAME_LEN);
        Uart_send(errorMsg);
      }
      // 重置接收状态，准备下一帧接收
      memset(uart_rx_buffer, 0, UART_BUFF_LEN);  // 清空缓冲区
      goRunning = 0;                   // 重置接收完成标志
      uart_rx_length = 0;              // 重置接收长度计数
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* 串口接收中断回调函数 */
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

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  //while (1)
  //{
  //}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/* USER CODE BEGIN 6 */

/* 重定向 fputc 函数到 USART1，使 printf 函数可以通过串口输出 */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/* USER CODE END 6 */
