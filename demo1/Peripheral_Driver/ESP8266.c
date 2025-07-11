#include "ESP8266.h"
#include <stdint.h>
#include "usart.h"
#include <string.h>
#include "stm32f1xx_hal.h"
#include "stdio.h"
#include "circle_buffer.h"



#define NETWORK_FLOOR  0       //11为0，8为1
#if NETWORK_FLOOR
#define NETWORK_NAME "ESP_route"
#else
#define NETWORK_NAME "ESP_TEST"
#endif



// #define NETWORK_NAME "ESP_route"
// #define NTEWORK_NAME "ESP_TEST"
#define NETWORK_PWD "123456789"
#define ESP_IP "192.168.8.2"
#define ESP_GATEWAY "192.168.8.1"
#define ESP_NETMASK "255.255.255.0"
#define CONNECT_IP "192.168.8.1"
#define ESP_PORT 9999
#define ESP_NUM 1

#define ESP_MODE 0    //MODE=1,means client mode,MODE=0 means server mode.
//uint16_t g_rx_datalen=0;
uint8_t flag_buf=0;
uint8_t debug_buffer[50]={0};



/*
flag == 0时，对应ESP8266初始化使用
flag == 1时，对应初始化结束，进行命令检测
flag == 2时，对应串口调试模式
*/
void Change_buf(uint8_t flag)
{
    flag_buf = flag;
}

uint8_t read_flag_buf(void)
{
    return flag_buf;
}

int sendATcmd(const char *cmd,const char *expectedRespond,uint16_t ms)
{
    uint8_t responod_buf[255]={0};
    uint16_t i=0;
    HAL_UART_Transmit(&huart1,(const uint8_t *)cmd,strlen(cmd),5);
    while(ms--)
    {
        HAL_Delay(1);       //delay 1ms
    }
    Waitfor_RX_COMPLT();

    while(g_buf.r!=g_buf.w)
    {
        circle_buf_read(&g_buf,&responod_buf[i++]);
    }
    #if DEBUG
    printf("receive data:%s\r\n",responod_buf);
    #endif // DEBUG

    
    if(strstr((const char *)responod_buf,expectedRespond)==NULL)
    {
        #if DEBUG
        printf("[ESP8266.c]:RX error\r\n");
        #endif
        return -1;
    }
    else 
    {   
        #if DEBUG
        printf("[ESP8266.c]:RX success!cmd:%s\r\n",cmd);
        #endif
        return 0;
    }
}


void ESP_sendata(uint32_t len,uint8_t *data)
{
    uint8_t cmd[50]={0};
    sprintf(cmd,"AT+CIPSEND=0,%d\r\n",len-1);
    HAL_UART_Transmit(&huart1,(const uint8_t *)cmd,strlen(cmd),10);
    HAL_Delay(10);  //中断被block住，调高systick优先级
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
    HAL_UART_Transmit(&huart1,(const uint8_t *)data,len,10);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
}

void ESP_sendata_ID(uint32_t len,uint8_t *data)
{
    uint8_t cmd[50]={0};
    // sprintf(debug_buffer,"AT+CIPSEND=%d,%d\r\n",client_id,len-1);
    sprintf(cmd,"AT+CIPSEND=%d,%d\r\n",client_id,len);
    HAL_UART_Transmit(&huart1,(const uint8_t *)cmd,strlen(cmd),10);
    HAL_Delay(10);  //中断被block住，调高systick优先级
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);
    HAL_UART_Transmit(&huart1,(const uint8_t *)data,len,10);
    HAL_UARTEx_ReceiveToIdle_IT(&huart1,buf_rx,50);


}

int ESP8266_Client_init(void)
{
    char cmd[256];
    flag_1=1;
    HAL_UART_Transmit(&huart1,(const uint8_t *)"+++",3,2);
    HAL_Delay(100);
    HAL_UART_Transmit(&huart1,(const uint8_t *)"\r\n",2,2);
    //HAL_UART_Transmit(&huart1,(const uint8_t *)"AT+RESTORE\r\n",13,2);
    //HAL_Delay(3000);
    //UART1_IDLE_INIT();
    HAL_Delay(200);
    //sendATcmd("AT+RESTORE\r\n","OK",4000);
    if(sendATcmd("AT\r\n","OK",5)!=0)
    {
        return -1;
    }
    #if ESP_MODE==1
    sendATcmd("AT+CWMODE=1\r\n","OK",10);
    sprintf(cmd,"AT+CWJAP=\"%s\",\"%s\"\r\n", NETWORK_NAME, NETWORK_PWD);
    sendATcmd(cmd,"OK",3000);
    sprintf(cmd,"AT+CIPSTA=\"%s\",\"%s\",\"%s\"\r\n", ESP_IP, ESP_GATEWAY, ESP_NETMASK);
    sendATcmd(cmd,"OK",1000);
    sendATcmd("AT+CIPMUX=0\r\n","OK",10);

    sendATcmd("AT+CIPMODE=1\r\n","OK",10);
    //sprintf(cmd,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", CONNECT_IP, ESP_PORT);
    sprintf(cmd,"AT+CIPSTART=\"UDP\",\"%s\",%d,%d,0\r\n", CONNECT_IP, ESP_PORT,ESP_PORT);

    if(sendATcmd(cmd,"CONNECT",2000)!=0)
    {
        printf("Check the server port %d open or not!!!",ESP_PORT);
        return -1;
    }
    sendATcmd("AT+CIPSEND\r\n","OK",10);
    HAL_UART_Transmit(&huart1,"hello user!\r\n",strlen("hello user!\r\n"),1);
    #else
    /*
    AT+CWMODE=1
    AT+CWJAP="ESP_TEST","123456789"
    AT+CIPSTA="192.168.137.98","192.168.137.1","255.255.255.0"
    AT+CIPMUX=1
    AT+CIPSERVER=1,9999
    ATE0
    
    */
 
    sendATcmd("AT+CWMODE=1\r\n","OK",10);
    sprintf(cmd,"AT+CWJAP=\"%s\",\"%s\"\r\n", NETWORK_NAME, NETWORK_PWD);
    sendATcmd(cmd,"OK",3000);
    sprintf(cmd,"AT+CIPSTA=\"%s\",\"%s\",\"%s\"\r\n", ESP_IP, ESP_GATEWAY, ESP_NETMASK);
    sendATcmd(cmd,"OK",1000);
    sendATcmd("AT+CIPMUX=1\r\n","OK",10);
    sprintf(cmd,"AT+CIPSERVER=1,%d\r\n",ESP_PORT);
    sendATcmd(cmd,"OK",200);
    sendATcmd("ATE0\r\n","OK",10);

    #endif

    
    
    Change_buf(1);
    
    return 0;
}
