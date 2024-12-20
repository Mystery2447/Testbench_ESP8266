#ifndef  _CIRCLE_BUFFER_H
#define _CIRCLE_BUFFER_H

typedef struct circle_buffer
{
    uint8_t *buffer;
    uint16_t r;
    uint16_t w;
    uint16_t len;
}circle_buf, *p_circle_buf;

void circle_buf_init(p_circle_buf a,uint16_t len,uint8_t *buf);
int circle_buf_write(p_circle_buf buf,uint8_t data);
int circle_buf_read(p_circle_buf buf,uint8_t *data);
int8_t circle_buf_readall(p_circle_buf buf,uint8_t *data);
extern circle_buf buf_rxfrom_usart3;//从USART3接收的缓冲区 为了与PC进行串口数据交换
extern char buffer_usart3[2000];
extern char buffer_tmp[512];

#endif // ! _CIRCLE_BUFFER_H