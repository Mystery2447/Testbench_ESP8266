#include "stm32f1xx_hal.h"
#include "Relay.h"
#include "gpio.h"

void Relay_init(void)
{
    HAL_GPIO_WritePin(Relay1_GPIO_Port,Relay1_Pin,GPIO_PIN_RESET);
}

void Relay_close_NC(void)
{
    HAL_GPIO_WritePin(Relay1_GPIO_Port,Relay1_Pin,GPIO_PIN_RESET);
}


void Relay_close_NO(void)
{
    HAL_GPIO_WritePin(Relay1_GPIO_Port,Relay1_Pin,GPIO_PIN_SET);
}

