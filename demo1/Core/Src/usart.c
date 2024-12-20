/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <stdio.h>
#include "ESP8266.h"
#include "circle_buffer.h"
volatile uint8_t g_rx_cmplt=0;
uint8_t buf_rx[50]={0};
uint8_t g_rx_buf[1024];
circle_buf g_buf;


volatile uint8_t flag_switchoff = 0;

uint8_t g_buf_cmd[50];
circle_buf g_cmd;
uint16_t g_rx_datalen=0;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 1);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


void Waitfor_RX_COMPLT(void)
{

    while (g_rx_cmplt == 0)
    {
      if(flag_switchoff)
      {
        flag_1=0;
        flag_switchoff = 0;
        ESP_sendata(strlen("[LOG]:switchoff ADCU successfully!\r\n")+1,"[LOG]:switchoff ADCU successfully!\r\n");      
      }
    }
    g_rx_cmplt = 0; // 重置标志�??
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart == &huart1)
    {
      if(flag_1)
      {
        g_rx_cmplt = 1;
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
        if(read_flag_buf()==1)
        {
          for(uint16_t i =0;i<Size;i++)
          {
            circle_buf_write(&g_cmd,buf_rx[i]);
          }
        }
        else
        {
          for(uint16_t a=0;a<Size;a++)
          {
            circle_buf_write(&g_buf,buf_rx[a]);
          }
        }
      }
      else
      {
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,buffer_tmp_cmd_raw,50);
        if(strstr(buffer_tmp_cmd_raw,"IPD,")!=NULL)
        {
          p_tmp_cmd = strchr(buffer_tmp_cmd_raw,':');
          p_tmp_cmd++;
          flag_3=1;
        }
      }
      
    }
    else if(huart == &huart3)
    {
      if(flag_1)
      {
        flag_2=0;
        if(findSubArray(buffer_mcuuart,sizeof(buffer_mcuuart),"enter shut_down!",strlen("enter shut_down!"))==0)
        {
          flag_switchoff = 1;
        }
        else 
        {
          HAL_UARTEx_ReceiveToIdle_IT(&huart3,buffer_mcuuart,200);
        }
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
      }
      else
      {
        HAL_UARTEx_ReceiveToIdle_IT(&huart3,buffer_tmp,2000);
        for (uint16_t i = 0; i < Size; i++)
        {
          circle_buf_write(&buf_rxfrom_usart3,buffer_tmp[i]);

          /* code */
        }
        
        flag_2=1;
      }

    }


}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart1)
    {
      if(flag_1)
      {
        g_rx_cmplt = 1;
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
        if(read_flag_buf()==1)
        {
          for(uint16_t i =0;i<50;i++)
          {
            circle_buf_write(&g_cmd,buf_rx[i]);
          }
        }
        else
        {
          for(uint16_t a=0;a<50;a++)
          {
            circle_buf_write(&g_buf,buf_rx[a]);
          }
        }
      }
      else
      {
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,buffer_tmp_cmd_raw,50);
        if(strstr(buffer_tmp_cmd_raw,"IPD,")!=NULL)
        {
          p_tmp_cmd = strchr(buffer_tmp_cmd_raw,':');
          p_tmp_cmd++;
          flag_3=1;
        }
      }

    }
    else if(huart == &huart3)
    {
      if(flag_1)
      {
        if(findSubArray(buffer_mcuuart,sizeof(buffer_mcuuart),"enter shut_down!",strlen("enter shut_down!")))
        {
          flag_switchoff = 1;
        }
        else 
        {
          HAL_UARTEx_ReceiveToIdle_IT(&huart3,buffer_mcuuart,200);
        }
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
      }
      else
      {
        HAL_UARTEx_ReceiveToIdle_IT(&huart3,buffer_tmp,2000);
        for (uint16_t i = 0; i < 512; i++)
        {
          circle_buf_write(&buf_rxfrom_usart3,buffer_tmp[i]);

          /* code */
        }
        
        flag_2=1;
      }


    }
  
}

void UART1_IDLE_INIT(void)
{
    circle_buf_init(&g_buf,1024,g_rx_buf);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
}


/* USER CODE END 1 */
