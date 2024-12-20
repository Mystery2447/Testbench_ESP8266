#include <stdint.h>
#include <stdio.h>
#include "circle_buffer.h"


circle_buf buf_rxfrom_usart3;
char buffer_tmp[512]={0};
char buffer_usart3[2000]={0};

void circle_buf_init(p_circle_buf a,uint16_t len,uint8_t *buf)
{
    a->buffer = buf;
    a->len = len;
    a->r = 0;
    a->w = 0;
}

int8_t circle_buf_readall(p_circle_buf buf,uint8_t *data)
{
    if(data==NULL)
    {
        return -1;
    }
    else
    {
        while(buf->r!=buf->w)
        {
            *data++= buf->buffer[buf->r];
            buf->r++;
            if(buf->r==buf->len)
            {
                buf->r=0;
            }
            return 0;
        }
        return -1;
    }
}

int circle_buf_write(p_circle_buf buf,uint8_t data)
{
    uint16_t next_w = buf->w+1;
    if(next_w==buf->len)
    {
        next_w=0;   
    }
    if(next_w != buf->r)
    {
        buf->buffer[buf->w] = data;
        buf->w = next_w;
        return 0;
    }
    else
    {
        return -1;
    }  
}

int circle_buf_read(p_circle_buf buf,uint8_t *data)
{
    if(buf->r != buf->w)
    {
        *data = buf->buffer[buf->r];
        buf->r++;
        if(buf->r == buf->len)
        {
            buf->r = 0;
        }
        return 0;

    }
    else return -1;
}
