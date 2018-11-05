/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <board.h>
#include <rtthread.h>

#ifdef RT_USING_DFS
/* dfs init */
#include <dfs_init.h>
/* dfs filesystem:ELM filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>
#endif

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f2x7_eth.h"
#endif


struct rx_msg
{
    rt_device_t dev;
    rt_size_t size;
};

////static rt_mq_t rx_mq;
////static struct rt_messagequeue  rx_mq;
static char uart2cmd_rx_buffer[64];
//static char msg_pool_cmd[512];

unsigned char test_switch = 1;

//static rt_timer_t timer=1;
 rt_timer_t timer1;
//static int cnt = 0;
/* 定时器1超时函数 */
static void timeout1(void *parameter)
{
//////    rt_tick_t timeout = 300;
//	rt_kprintf("periodic timer is timeout\n");
	test_switch = 1;
//	rst_n_xor;
	p_gate_L;
	if (timer1 != RT_NULL) rt_timer_stop(timer1);

    /* ??10? */
//////    if (cnt++ >= 10)
//////    {
//////        rt_timer_control(timer1, RT_TIMER_CTRL_SET_ONESHOT, (void *)&timeout);
//////    }
}


//////rt_err_t uart_input(rt_device_t dev, rt_size_t size)
//////{
//////    struct rx_msg msg;
//////    msg.dev = dev;
//////    msg.size = size;

//////    rt_mq_send(rx_mq, &msg, sizeof(struct rx_msg));

//////    return RT_EOK;
//////}


void rt_led_thread_entry(void* parameter)
{
  rt_uint8_t i,j;
  rt_hw_led_init();
  

//////////////////////////////////////
  for(i=0;i<5;i++)
  {  
    for(j=0;j<2;j++)
    {  
      led(RunLed+j,LedOn);
      rt_thread_delay(10);
      led(RunLed+j,LedOff);
      rt_thread_delay(10);
    }  
  } 
  while(1)
  { 
    led(RunLed,LedOn);
    rt_thread_delay(50);
    led(RunLed,LedOff);
    rt_thread_delay(50);    
  }
}


void rt_cmd_uart_proc_thread_entry(void* parameter)
{
	while(1)
	{
//		if(test_switch)
//		{
//			test_switch = 0;
//			if (timer1 != RT_NULL) rt_timer_start(timer1);
//		}
		/*调用测频模块，开启测频时序及读取数据*/
		frequence_measures();
		
		rt_thread_delay(15);
	}
//////	struct rx_msg msg;
//////  int count = 0;
//////  rt_device_t device, write_device;
//////  rt_err_t result = RT_EOK;
//////	
//////	device = rt_device_find("uart2");
//////	
//////	if (device!= RT_NULL)
//////	{
//////		rt_device_set_rx_indicate(device, uart_input);
//////    rt_device_open(device, RT_DEVICE_OFLAG_RDWR|RT_DEVICE_FLAG_INT_RX);
//////	}
//////	
//////	write_device = device;
//////	
//////	while(1)
//////	{
//////		result = rt_mq_recv(rx_mq, &msg, sizeof(struct rx_msg), 50);
//////    if (result == -RT_ETIMEOUT)
//////		{
//////			rt_kprintf("timeout count:%d\n", ++count);
//////    }
//////		
//////		if (result == RT_EOK)
//////    {
//////			rt_uint32_t rx_length;
//////      rx_length = (sizeof(uart2cmd_rx_buffer) - 1) > msg.size ?
//////                    msg.size : sizeof(uart2cmd_rx_buffer) - 1;
//////			rx_length = rt_device_read(msg.dev, 0, &uart2cmd_rx_buffer[0],
//////                    rx_length);
//////      uart2cmd_rx_buffer[rx_length] = '\0';

//////      if (write_device != RT_NULL)
//////                rt_device_write(write_device, 0, &uart2cmd_rx_buffer[0],
//////                        rx_length);
//////    }
//////	}
//////	
}


void rt_init_thread_entry(void* parameter)
{
/* Filesystem Initialization */
#ifdef RT_USING_DFS
	{
		/* init the device filesystem */
		dfs_init();

#ifdef RT_USING_DFS_ELMFAT
		/* init the elm chan FatFs filesystam*/
		elm_init();

		/* mount sd card fat partition 1 as root directory */
		if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
		{
			rt_kprintf("File System initialized!\n");
		}
		else
			rt_kprintf("File System initialzation failed!\n");
#endif
	}
#endif

/* LwIP Initialization */
#ifdef RT_USING_LWIP
	{
		extern void lwip_sys_init(void);

		/* register ethernetif device */
		eth_system_device_init();

		/* initialize eth interface */
		rt_hw_stm32_eth_init();

		/* init lwip system */
		lwip_sys_init();
		rt_kprintf("TCP/IP initialized!\n");
	}
#endif
}

//////static struct rt_messagequeue s_your_mq;//?????????
//////static char s_msg_pool[1024];//???

//static rt_timer_t timer1;
 rt_timer_t timer1;
int rt_application_init()
{
	rt_thread_t tid;

//////  rt_mq_init(&s_your_mq,"test",&s_msg_pool[0],sizeof(struct rx_msg),sizeof(s_msg_pool),RT_IPC_FLAG_FIFO); 
//////	rx_mq =&s_your_mq;//?rx_mq???s_your_mq
	
//markoi	tid = rt_thread_create("init",
//								rt_init_thread_entry, RT_NULL,
//								2048, RT_THREAD_PRIORITY_MAX/3, 20);

//	if (tid != RT_NULL)
//markoi		rt_thread_startup(tid);
	
	tid = rt_thread_create("led_flash",   
					                        rt_led_thread_entry, RT_NULL,
					                        512, 16, 5);
	if (tid != RT_NULL)
	  rt_thread_startup(tid); 
	
	tid = rt_thread_create("cmd_uart_proc",
	                                rt_cmd_uart_proc_thread_entry, RT_NULL,
								                  2048, 10, 5);
	if (tid != RT_NULL)
		rt_thread_startup(tid); 
	
  timer1 = rt_timer_create("timer1", /* 定时器名字是 timer1 */
                             timeout1, /* 超时时回调的处理函数 */
                             RT_NULL, /* 超时函数的入口参数 */
                             100, /* 定时长度，以OS Tick为单位，即10个OS Tick */
                             RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */




	return 0;
}

/*@}*/
