#ifndef __FREQUENCE_H
#define __FREQUENCE_H
#include <rtthread.h>


#define p_gate_rcc                  RCC_AHB1Periph_GPIOA
#define p_gate_gpio                 GPIOA
#define p_gate_pin                  (GPIO_Pin_7)

#define ready_rcc                   RCC_AHB1Periph_GPIOC
#define ready_gpio                  GPIOC
#define ready_pin                   (GPIO_Pin_5)

#define read_clk_rcc                RCC_AHB1Periph_GPIOC
#define read_clk_gpio               GPIOC
#define read_clk_pin                (GPIO_Pin_4)

#define clr_rcc                     RCC_AHB1Periph_GPIOB
#define clr_gpio                    GPIOB
#define clr_pin                     (GPIO_Pin_1)

#define sel_rcc                     RCC_AHB1Periph_GPIOB
#define sel_gpio                    GPIOB
#define sel_pin                     (GPIO_Pin_0)

#define rst_n_rcc                   RCC_AHB1Periph_GPIOE
#define rst_n_gpio                  GPIOE
#define rst_n_pin                   (GPIO_Pin_7)

#define r_gate_rcc                   RCC_AHB1Periph_GPIOA
#define r_gate_gpio                  GPIOA
#define r_gate_pin                   (GPIO_Pin_6)

#define sn_rcc                       RCC_AHB1Periph_GPIOD
#define sn_gpio                      GPIOD
#define sn_pin                       GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10

#define freq_data_rcc               RCC_AHB1Periph_GPIOE
#define freq_data_gpio              GPIOE
#define freq_data_pin                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15

#define p_gate_H          (GPIO_SetBits(p_gate_gpio,p_gate_pin ))
#define p_gate_L          (GPIO_ResetBits(p_gate_gpio,p_gate_pin))
#define ready_H           (GPIO_SetBits(ready_gpio,ready_pin))
#define ready_L           (GPIO_ResetBits(ready_gpio,ready_pin))

#define read_clk_H        (GPIO_SetBits(read_clk_gpio,read_clk_pin ))
#define read_clk_L        (GPIO_ResetBits(read_clk_gpio,read_clk_pin))
#define clr_H             (GPIO_SetBits(clr_gpio,clr_pin))
#define clr_L             (GPIO_ResetBits(clr_gpio,clr_pin))

#define sel_H             (GPIO_SetBits(sel_gpio,sel_pin ))
#define sel_L             (GPIO_ResetBits(sel_gpio,sel_pin))
#define rst_n_H           (GPIO_SetBits(rst_n_gpio,rst_n_pin))
#define rst_n_L           (GPIO_ResetBits(rst_n_gpio,rst_n_pin))

#define rst_n_xor          rst_n_gpio->ODR^=rst_n_pin


#define r_gate_read       r_gate_gpio->IDR & GPIO_Pin_8
#define sn_read           ((sn_gpio->IDR & 0x0700) >> 8)
#define freq_data_read    ((freq_data_gpio->IDR & 0xFF00) >> 8)

extern unsigned char test_switch;
extern rt_timer_t timer1;
extern void rt_hw_freq_init(void);
extern void frequence_measures(void);






#endif
