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



#endif // ! _CIRCLE_BUFFER_H