//#include "usart.h"
#include <stm32f2xx.h>
#include <board.h>
//#include "external.h"


#define led1_rcc                    RCC_AHB1Periph_GPIOA
#define led1_gpio                   GPIOA
#define led1_pin                    (GPIO_Pin_0)

#define led2_rcc                    RCC_AHB1Periph_GPIOA
#define led2_gpio                   GPIOA
#define led2_pin                    (GPIO_Pin_1)

void rt_hw_led_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_AHB1PeriphClockCmd(led1_rcc, ENABLE);
  GPIO_InitStruct.GPIO_Pin = led1_pin | led2_pin;; //PA1,PA0
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(led1_gpio, &GPIO_InitStruct);
}

void rt_hw_led_off(rt_uint8_t n)
{
	switch (n)
		{
		case 0:
        GPIO_SetBits(led1_gpio, led1_pin);
        break;
    case 1:
        GPIO_SetBits(led2_gpio, led2_pin);
        break;
    default:
        break;
    }  
}

void rt_hw_led_on(rt_uint8_t n)
{
  switch(n)
  {
  case 0:
    GPIO_ResetBits(led1_gpio, led1_pin);
    break;
  case 1:
    GPIO_ResetBits(led2_gpio, led2_pin);
    break;

  default:
    break;
  }
}


static rt_uint8_t led_inited = 0;
void led(rt_uint32_t led, rt_uint32_t value)
{
    /* init led configuration if it's not inited. */
    if(!led_inited)
    {
        rt_hw_led_init();
        led_inited = 1;
    }
    if( led < 2 )
    {
        /* set led status */
        switch (value)
        {
        case 0:
            rt_hw_led_off(led);
            break;
        case 1:
            rt_hw_led_on(led);
            break;
        default:
            break;
        }
    }
}
#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(led, set led(1,0/1) on-1 or off-0)
#endif

