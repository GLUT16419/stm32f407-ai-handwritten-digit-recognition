/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* 串口通信缓冲区配置 */
#define UART_BUFF_LEN 1024          // 串口接收缓冲区总长度
#define ONE_FRAME_LEN 1+784+2       // 单帧数据长度：1（帧头）+784（28*28手写数据）+2（帧尾）

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* 串口通信全局变量声明 */
extern uint16_t uart_rx_length;        // 已接收数据长度计数
extern uint8_t uart_rx_byte;           // 单次接收字节缓存
extern uint8_t uart_rx_buffer[UART_BUFF_LEN];  // 接收缓冲区（存储上位机发送的完整数据）
extern volatile uint8_t goRunning;     // 接收完成标志（0：未完成，1：已完成一帧接收）

/* 串口发送函数声明 */
void Uart_send(char * str);

/* 数据格式转换函数声明 */
void PictureCharArrayToFloat(uint8_t *srcBuf, float *dstBuf, int len);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
