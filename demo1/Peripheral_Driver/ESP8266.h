#ifndef  _ESP8266_H
#define _ESP8266_H
#define DEBUG 1
#include <stdint.h>
void ESP_sendata(uint32_t len,uint8_t *data);
int sendATcmd(const char *cmd,const char *expectedRespond,uint16_t ms);
void ESP_sendata_ID(uint32_t len,uint8_t *data);
int ESP8266_Client_init(void);
uint8_t read_flag_buf(void);
void Change_buf(uint8_t flag);

extern uint8_t debug_buffer[50];

#endif // ! _ESP8266_H
