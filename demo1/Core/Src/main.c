/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "circle_buffer.h"
#include "Relay.h"
#include "ESP8266.h"
#include <stdio.h>
#include <string.h>
#define CLIENT_ID 1
//char cmd_raw[50]={0};
//char cmd_filter[50]={0};
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char buffer_mcuuart[200]={0};
volatile uint8_t client_id=0;
uint8_t flag_1=0;
uint8_t *p_tmp_cmd=NULL;
volatile uint8_t flag_2=0;//usart3接收完成标志位
char buffer_tmp_cmd_raw[50]={0};//暂时只支持mcu串口
uint8_t flag_3=0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  
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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  Relay_init();
  //HAL_UART_Transmit(&huart1,(const uint8_t *)"AT+RESTORE\r\n",13,2);
  //HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
  //HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
  UART1_IDLE_INIT();
  circle_buf_init(&g_cmd,50,g_buf_cmd);
  HAL_Delay(500);
  if(ESP8266_Client_init()!=0)
  {
    printf("ESP8266 init Fail!!!");
  }
  else  printf("ESP8266 init success!!!");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    
    /* USER CODE BEGIN 3 */
    process_ESP_data();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

int findSubArray(const char *array, size_t array_len, const char *subarray, size_t subarray_len)
{
  if (subarray_len == 0 || array_len < subarray_len) {
        return -1;
    }

    for (size_t i = 0; i <= array_len - subarray_len; ++i) {
        size_t j;
        for (j = 0; j < subarray_len; ++j) {
            if (array[i + j] != subarray[j]) {
                break;
            }
        }
        if (j == subarray_len) {
            return 0; // 找到子数组
        }
    }
    return -1;




}

char* my_strchr(char *str,char c)
{

  while(*str!='\0')
  {
    if(c == *str)
    {
      return str;
    }
    str++;
  }
  if(*str == c)
  {
    return str;
  }
  return NULL;
}

uint8_t get_ClientID(char *p)
{
  uint8_t id =0;
  uint8_t *ptr =NULL;
  ptr = my_strchr(p,',');
  ptr++;
  if(ptr!=NULL)
  {
    id = *ptr;
  }
  return id-48 ;
}

void my_strcpy(char *dest,char *src)
{
  while(*src!='\0')
  {
    *dest++ = *src++;
  }
  *dest = '\0';
}

void process_ESP_data(void)
{
  char cmd_raw[50]={0};
  char cmd_filter[50]={};
  uint8_t lenth = 0;
  char *p;
  

  Waitfor_RX_COMPLT();

  while(g_cmd.r != g_cmd.w)
  {
    circle_buf_read(&g_cmd,(uint8_t *)&cmd_raw[lenth++]);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
  }
  
  if(strstr(cmd_raw,"IPD,")!=NULL)
  {
    client_id = get_ClientID(cmd_raw);
    p = strchr(cmd_raw,':');
    p++;
    my_strcpy(cmd_filter,p);
    handle_cmd((uint8_t *)cmd_filter);
  }

}


void KL15OFF()
{
  flag_1=1;
  HAL_UART_Transmit(&huart3,"poweroff\r\n",strlen("poweroff\r\n"),10);

  #if CLIENT_ID
  ESP_sendata_ID(strlen("KL15 OFF trigger,pls waiting for MCU shutdown...\r\n\r\n"),"KL15 OFF trigger,pls waiting for MCU shutdown...\r\n\r\n");
  #else
  ESP_sendata(strlen("KL15 OFF trigger,pls waiting for MCU shutdown...\r\n"),"KL15 OFF trigger,pls waiting for MCU shutdown...\r\n");
  #endif     


  Relay_close_NO();
  HAL_UARTEx_ReceiveToIdle_IT(&huart3,buffer_mcuuart,200);
  // HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50); //尝试下


  
}

void handle_cmd(uint8_t *cmd)
{
  #if DEBUG
  //printf("receive CMD is[%s]",cmd);
  #endif
  if(strncmp((const char *)cmd,"poweron",strlen("poweron"))==0)//唤醒MCU
  {
    
    printf("MCU wakeup\r\n");
    //HAL_UART_Transmit(&huart1,"AT+CIPSEND=0,40\r\n",strlen("AT+CIPSEND=0,40\r\n"),2);
    //HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
    //HAL_Delay(10);
    HAL_UART_Transmit(&huart3,"poweron\r\n",sizeof("poweron\r\n"),10);
    //HAL_UART_Transmit(&huart1,"Already poweron!pls waiting few sec...\r\n",strlen("Already poweron!pls waiting few sec...\r\n"),2);
    #if CLIENT_ID
    ESP_sendata_ID(strlen("[LOG]:MCU already poweron\r\n\r\n"),"[LOG]:MCU already poweron\r\n\r\n");
    #else
    ESP_sendata(strlen("[LOG]:MCU already poweron\r\n"),"[LOG]:MCU already poweron\r\n");
    #endif
    

    //HAL_UART_Transmit(&huart1,"[LOG]:MCU already poweron\r\n",strlen("[LOG]:MCU already poweron\r\n"),2);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);




  }
  else if (strncmp((const char *)cmd,"switchon ADCU",strlen("switchon ADCU"))==0)//域控上电
  {
    printf("ADC switch on\r\n");
    Relay_close_NC();
    //HAL_UART_Transmit(&huart1,"AT+CIPSEND=0,21\r\n",strlen("AT+CIPSEND=0,21\r\n"),2);
    //HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
    //HAL_Delay(10);
    //HAL_UART_Transmit(&huart1,"[LOG]:switchon ADCU\r\n",strlen("[LOG]:switchon ADCU\r\n"),2);
    #if CLIENT_ID
    ESP_sendata_ID(strlen("[LOG]:switchon ADCU successfully!\r\n\r\n"),"[LOG]:switchon ADCU successfully!\r\n\r\n");
    #else
    ESP_sendata(strlen("[LOG]:switchon ADCU successfully!\r\n"),"[LOG]:switchon ADCU successfully!\r\n");
    #endif

    //HAL_UART_Transmit(&huart1,"[LOG]:switchon ADCU\r\n",strlen("[LOG]:switchon ADCU\r\n"),2);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
  }
  else if (strncmp((const char *)cmd,"switchoff ADCU",strlen("switchoff ADCU"))==0)//域控掉电
  {
    printf("ADC switch off\r\n");
    


    KL15OFF();  //KL15方案



    // ESP_sendata(strlen("[LOG]:switchoff ADCU successfully!\r\npls wait for more than 10s for power discharge before turning on")+1,"[LOG]:switchoff ADCU successfully!\r\npls wait for more than 10s for power discharge before turning on");
    //HAL_UART_Transmit(&huart1,"[LOG]:switchoff ADCU\r\n",strlen("[LOG]:switchoff ADCU\r\n"),2);
    //HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
  }
  else if(strncmp((const char *)cmd,"\r\n",strlen("\r\n"))==0)
  {
  ESP_sendata_ID(strlen("\r\n"),"\r\n");
  }
  else if(strncmp((const char *)cmd,"soc uart debug",strlen("soc uart debug"))==0)//串口debug回读功能
  {
    //circle——buffer初始化
    circle_buf_init(&buf_rxfrom_usart3,2000,buffer_usart3);
    HAL_UARTEx_ReceiveToIdle_IT(&huart3,buffer_tmp,2000);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buffer_tmp_cmd_raw,50);
    flag_1=0;
    for(;;)
    {
      char buffer_tmp[512]={0};
      uint8_t flag_exit=0;
      uint16_t i=0;
      while(flag_2==0)//等待串口3接收完成
      {
        if(flag_3)
        {
          flag_3=0;
          if(strncmp((const char *)p_tmp_cmd,"exit()",strlen("exit()"))==0)
          {
            flag_1=1;
            flag_exit=1;
            break;
          }
          else
          {
            HAL_UART_Transmit(&huart3,(const uint8_t*)p_tmp_cmd,strlen(p_tmp_cmd),10);
          }
        }


      }
      if(flag_exit)
      {
        flag_exit=0;
        break;
      }

      flag_2=0;
      while(buf_rxfrom_usart3.r!=buf_rxfrom_usart3.w)
      {
        circle_buf_read(&buf_rxfrom_usart3,&buffer_tmp[i++]);

      }
      ESP_sendata_ID(i,buffer_tmp);



    }








  }
  else
  {
    #if CLIENT_ID
    ESP_sendata_ID(strlen("[WARNING]:invalid cmd!!!\r\n\r\n"),"[WARNING]:invalid cmd!!!\r\n\r\n");
    #else
    ESP_sendata(strlen("[WARNING]:invalid cmd!!!\r\n"),"[WARNING]:invalid cmd!!!\r\n");
    #endif     
    

    //HAL_UART_Transmit(&huart1,"[WARNING]:invalid cmd!!!\r\n",strlen("[WARNING]:invalid cmd!!!\r\n"),2);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);

  }

}


int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handle;r_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {

  }
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
