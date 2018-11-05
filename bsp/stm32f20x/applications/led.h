/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H
#include <rtdef.h>                      
void rt_hw_led_init(void);
void rt_hw_led_off(unsigned char n);
void rt_hw_led_on(unsigned char  n);
//void rt_hw_speaker_init(void);
//void rt_hw_Speaker_off(void);
//void rt_hw_Speaker_on(void);
void led(rt_uint32_t led, rt_uint32_t value);

void rt_hw_io_init(void);

/**********************End****************/
#endif
