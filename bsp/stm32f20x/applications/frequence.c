#include "stm32f2xx.h"
#include <rtthread.h> 
#include <board.h>
#include <string.h>

unsigned int numx = 0;
unsigned int numo = 0;

void rt_hw_us_delay(rt_uint32_t us)
{
	rt_uint32_t delta;
  rt_uint32_t current_delay;
  us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
	 /* 获得当前时间 */
  delta = SysTick->VAL;
  /* 循环获得当前时间，直到达到指定的时间后退出循环 */
  do
	{
		if ( delta > SysTick->VAL )
			current_delay = delta - SysTick->VAL;
		else
     /* 延时跨越了一次OS tick的边界时的处理 */
			current_delay = SysTick->LOAD + delta - SysTick->VAL;
	}while( current_delay < us );
}

//static void us_delay(rt_uint16_t us) 
//{
//	rt_uint16_t  i=80,j;      
//  for(j=0;j<us;j++)
//  {
//		while(i)
//		{
//			i--;
//		}
//	}
//}

void rt_hw_freq_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  RCC_AHB1PeriphClockCmd(p_gate_rcc, ENABLE);
  GPIO_InitStruct.GPIO_Pin = p_gate_pin; //PA1,PA0
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(p_gate_gpio, &GPIO_InitStruct);
	

  RCC_AHB1PeriphClockCmd(ready_rcc, ENABLE);
  GPIO_InitStruct.GPIO_Pin = ready_pin | read_clk_pin; //PC5,PC4
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(ready_gpio, &GPIO_InitStruct);
	
	RCC_AHB1PeriphClockCmd(clr_rcc, ENABLE);
  GPIO_InitStruct.GPIO_Pin = clr_pin | sel_pin; //PB1,PB0
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(clr_gpio, &GPIO_InitStruct);
	
	RCC_AHB1PeriphClockCmd(rst_n_rcc, ENABLE);
  GPIO_InitStruct.GPIO_Pin = rst_n_pin; //PE7
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(rst_n_gpio, &GPIO_InitStruct);
	
	RCC_AHB1PeriphClockCmd(r_gate_rcc, ENABLE);
  GPIO_InitStruct.GPIO_Pin = r_gate_pin; //PA6
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
  GPIO_Init(r_gate_gpio, &GPIO_InitStruct);
	
	RCC_AHB1PeriphClockCmd(sn_rcc, ENABLE);
  GPIO_InitStruct.GPIO_Pin = sn_pin; //P7
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
  GPIO_Init(sn_gpio, &GPIO_InitStruct);
	
	RCC_AHB1PeriphClockCmd(freq_data_rcc, ENABLE);
  GPIO_InitStruct.GPIO_Pin = freq_data_pin; //P7
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_NOPULL;
  GPIO_Init(freq_data_gpio, &GPIO_InitStruct);
	
	p_gate_L;
	ready_L;
	read_clk_L;
	clr_L;
	sel_L;
	rst_n_H;	
}

void frequence_measures(void)
{
	signed char i=0;
	unsigned char sn =0;
	unsigned int data=0;
	unsigned int numo_temp=0;
	unsigned int numx_temp=0;
	float res = 0;
	char buffer_res[99]={0};
	rst_n_L;
	rt_hw_us_delay(20);
//	us_delay(20);
	rst_n_H;
	clr_H;
	rt_hw_us_delay(20);
//	us_delay(20);
	clr_L;
	rt_hw_us_delay(20);
//	us_delay(20);
	if(test_switch)
	{
		test_switch = 0;
		p_gate_H;
		if (timer1 != RT_NULL) rt_timer_start(timer1);
	}
	while(!test_switch)
	{
		rt_thread_delay(10);
	}
  ready_H;
	rt_hw_us_delay(20);
	for(i=0;i<8;i++)
	{
	  if(i==0)
		{
			ready_H;
			rt_hw_us_delay(20);
		}
		read_clk_H;
		rt_hw_us_delay(20);
//		us_delay(20);
		read_clk_L;

		
		sn = sn_read;
		rt_hw_us_delay(20);
//		us_delay(20);
		if(sn==i)
		{
			data = 0;
			data = freq_data_read;
			if(i<4)
			{
				numo_temp += data<<i*8;
				if(i==3)
				{
					numo = numo_temp;
					numo_temp = 0;
					rt_kprintf("the counter of fo is %u\n",numo);
				}
			}
			else
			{
				numx_temp += data<<(i-4)*8;
				if(i==7)
				{
					numx = numx_temp;
					numx_temp = 0;
					ready_L;
					rt_kprintf("the counter of fx is %u\n",numx);
					res = (double)numx / (double)numo * 50000000;
					sprintf(buffer_res,"the frequence of fx is %f\n\n", res);
					rt_kprintf("%s", buffer_res);
					memset(buffer_res, 99 ,0);
				}
			}
		}
		else
		{
			numo_temp = 0;
			numx_temp = 0;
			i = -1;
			ready_L;
			read_clk_H;
		  rt_hw_us_delay(20);
//		us_delay(20);
		  read_clk_L;
			rt_hw_us_delay(20);
			
		}
//		rt_thread_delay(50);
		
	}
	
	
	
}

