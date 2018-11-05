**************************************************readme***********************************************************
2018.06.22 
1 修改finsh的输入输出串口，由串口1改为串口2  (keil 5.14)
                  TXD              RXD
  串口1          PA9（PIN68）     PA10（PIN69）   
  串口2          PA2（PIN25）     PA3（PIN26）
  修改uart2作为finsh的设备有三处设置修改，另外还有一些串口的初始化,uart时钟总线管脚中断等设置
  a // <o> Console on USART: <0=> no console <1=>USART 1 <2=>USART 2 <3=> USART 3
    // 	<i>Default: 1
    //markoi #define STM32_CONSOLE_USART		1
    #define STM32_CONSOLE_USART		2 //markoi
  
  b /* SECTION: Device System */
    /* Using Device System */
    #define RT_USING_DEVICE
    //markoi #define RT_USING_UART1
     #define RT_USING_UART2 //markoi

  c #ifdef RT_USING_FINSH
	/* init finsh */
	finsh_system_init();
    //markoi finsh_set_device("uart1");
    finsh_set_device("uart2"); //markoi
    #endif


2018.06.23
1 增加运行指示灯

   D8     LED1      PA0(PIN23)         workstatus LED
   D7     LED2      PA1(PIN24)         
   
2 EEPROM参数读写
  参数地址
  参数数据结构
     内外标频选择    bool
	 标频            unsigned int 32bit   单位 HZ
	 参考闸门时间    unsigned short 16bit 单位 1mS
	 周期频率选择    bool
	 
2018.06.25
1（201806251624备份） 内容finsh设备uart2 
2 为LED函数增加钩子函数
3 改finsh设备uart1为uart2,准备将uart2作为来自上位机命令的收发串口
  uart2中断接收

2018.07.02
1 uart2接受来自上位机命令(参考RT手册内第六章的例程#缺初始化可参考网络“串口收发的例程，出现问题”)，快速发http://www.cmsoft.cn
  五次，只能收到三次。所以有遗漏。缓冲区读写问题修改。同时考虑串口操作直接读写的问题。
  
2018.08.01
1 增加定时器
  初始化口线
  
2018.11.05
上传到github
  
  

	 
	 
  
  

