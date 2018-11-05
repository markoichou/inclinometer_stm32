#ifndef __AT24C16A_H
#define __AT24C16A_H
#include <rtdef.h> 
#define TRUE           1
#define FALSE          0
#define SCL_RCC        RCC_AHB1Periph_GPIOA
#define SDA_RCC        RCC_AHB1Periph_GPIOC
//////////////////////////////////////////////
#define WP_RCC         RCC_AHB1Periph_GPIOC
#define WP_GPIO        GPIOC
#define WP_Pin         GPIO_Pin_8
#define WP_EN          (GPIO_SetBits(WP_GPIO,WP_Pin))
#define WP_DISEN       (GPIO_ResetBits(WP_GPIO,WP_Pin))
//////////////////////////////////////////////
#define SCL_GPIO       GPIOA
#define SCL_Pin        GPIO_Pin_8
#define SDA_GPIO       GPIOC
#define SDA_Pin        GPIO_Pin_9
#define SCL_H          (GPIO_SetBits(SCL_GPIO,SCL_Pin))
#define SCL_L          (GPIO_ResetBits(SCL_GPIO,SCL_Pin))
#define SDA_H          (GPIO_SetBits(SDA_GPIO,SDA_Pin))
#define SDA_L          (GPIO_ResetBits(SDA_GPIO,SDA_Pin))
#define SCL_read       GPIOA->IDR & GPIO_Pin_8
#define SDA_read       GPIOC->IDR & GPIO_Pin_9



#define    Sla_Wr      	0xa0    	         /*0000 0 0 0 0*/
#define    Sla_Rd      	0xa1    	         /*0000 0 0 0 1*/
#define    I2C1_SLAVE_ADDRESS7   0XA0
#define    I2C_PageSize          16
#define    I2C_BlockSize         128


extern void rt_hw_I2C_init(void);
extern unsigned char I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_Ack(void);
extern void I2C_NoAck(void);
extern unsigned char I2C_WaitAck(void); //返回为:=1有ACK,=0无ACK
extern void I2C_SendByte(rt_uint8_t SendByte); //数据从高位到低位//
extern rt_uint8_t I2C_ReceiveByte(void); //数据从高位到低位//
extern unsigned char I2C_FRAM_PageWrite(rt_uint8_t* pBuffer, rt_uint16_t WriteAddr, rt_uint16_t NumByteToWrite);
extern unsigned char I2C_FRAM_BufferWrite(rt_uint8_t* pBuffer, rt_uint16_t WriteAddr,rt_uint16_t NumByteToWrite);
extern unsigned char I2C_FRAM_BufferRead(rt_uint8_t* pBuffer, rt_uint16_t WriteAddr, rt_uint16_t NumByteToRead);

extern void Read_PressrePara(rt_uint8_t channel);    
extern void Save_PressrePara(rt_uint8_t channel);
extern void Read_Para_T(rt_uint8_t channel); 
extern void Save_Para_T(rt_uint8_t channel);







#define    channel1_Address      	0x10    	 //Pressure1参数地址  
#define    channel2_Address      	0x40    	 //Pressure2参数地址
#define    channel3_Address      	0x70    	 //Pressure3参数地址
#define    channel4_Address      	0xA0    	 //Pressure4参数地址










#endif
