#include "stm32f2xx.h"
#include <rtthread.h> 
#include <board.h>



/************************************************************************************************************************
**? ? ?:  	static I2C_delay(rt_uint8_t us)                    
**????:  	us??????
**??	?:	?		
**?	?:	??????
**????:
**              2011.11.7??
************************************************************************************************************************/
static void I2C_delay(rt_uint16_t us) 
{
	rt_uint16_t  i=80,j;      
  for(j=0;j<us;j++)
  {
		while(i)
		{
			i--;
		}
	}
}

/************************************************************************************************************************
**? ? ?:  	I2C_Start(void)                   
**????:  	??I2C
**??	?:	?		
**?	?:	??????
**????:
**              2011.11.7??
************************************************************************************************************************/
unsigned char I2C_Start(void)
{

	SDA_H;
	SCL_H;
	I2C_delay(5);
	if(!SDA_read)
        {
           return FALSE; //SDA?????????,??
        }
	SDA_L;
	I2C_delay(5);
	if(SDA_read) 
        {
          return FALSE; //SDA??????????,??
        }
	SDA_L;
	I2C_delay(5);
	return TRUE;
}

/************************************************************************************************************************
**? ? ?:  	I2C_Stop(void)                  
**????:  	??I2C
**??	?:	?		
**?	?:	??????
**????:
**              2011.11.7??
************************************************************************************************************************/
void I2C_Stop(void)
{
	SCL_L;
	I2C_delay(5);
	SDA_L;
	I2C_delay(5);
	SCL_H;
	I2C_delay(5);
	SDA_H;
	I2C_delay(5);
}

/************************************************************************************************************************
**? ? ?:  	I2C_Ack(void)                 
**????:  	I2C??
**??	?:	?		
**?	?:	??????
**????:
**              2011.11.7??
************************************************************************************************************************/
void I2C_Ack(void)
{
	SCL_L;
	I2C_delay(5);
	SDA_L;
	I2C_delay(5);
	SCL_H;
	I2C_delay(5);
	SCL_L;
	I2C_delay(5);
}


/************************************************************************************************************************
**? ? ?:  	I2C_NoAck(void)                
**????:  	I2C??
**??	?:	?		
**?	?:	??????
**????:
**              2011.11.7??
************************************************************************************************************************/
void I2C_NoAck(void)
{
	SCL_L;
	I2C_delay(5);
	SDA_H;
	I2C_delay(5);
	SCL_H;
	I2C_delay(5);
	SCL_L;
	I2C_delay(5);
}

/************************************************************************************************************************
**? ? ?:  	I2C_WaitAck(void)                
**????:  	
**??	?:	?		
**?	?:	??????
**????:
**              2011.11.7??
************************************************************************************************************************/
unsigned char I2C_WaitAck(void) //???:=1?ACK,=0?ACK
{
	SCL_L;
	I2C_delay(5);
	SDA_H;
	I2C_delay(5);
	SCL_H;
	I2C_delay(5);
	if(SDA_read)
	{
	SCL_L;
	return FALSE;
	}
	SCL_L;
	return TRUE;
}

/************************************************************************************************************************
**? ? ?:  	void I2C_SendByte(rt_uint8_t SendByte)               
**????:  	
**??	?:	?		
**?	?:	???????
**????:
**              2011.11.7??
************************************************************************************************************************/
void I2C_SendByte(rt_uint8_t SendByte) //????????//
{
	rt_uint8_t i=8;
	while(i--)
	{
	SCL_L;
	I2C_delay(5);
	if(SendByte&0x80)
	SDA_H;
	else
	SDA_L;
	SendByte<<=1;
	I2C_delay(5);
	SCL_H;
	I2C_delay(5);
	}
	SCL_L;
}

/************************************************************************************************************************
**? ? ?:  	I2C_ReceiveByte(void)             
**????:  	
**??	?:	?		
**?	?:	???????
**????:
**              2011.11.7??
************************************************************************************************************************/
rt_uint8_t I2C_ReceiveByte(void) //????????//
{
	rt_uint8_t i=8;
	rt_uint8_t ReceiveByte=0;

	SDA_H;
	while(i--)
	{
	ReceiveByte<<=1;
	SCL_L;
	I2C_delay(5);
	SCL_H;
	I2C_delay(5);
	if(SDA_read)
	{
	ReceiveByte|=0x01;
	}
	}
	SCL_L;
	return ReceiveByte;
}

/************************************************************************************************************************
**? ? ?:  	I2C_delay_ms(rt_uint16_t nms)             
**????:  	
**??	?:	?		
**?	?:	????
**????:
**              2011.11.7??
************************************************************************************************************************/
static void I2C_delay_ms(rt_uint16_t nms)
{
  rt_uint16_t x=0;
   while(nms--)
   {
     x=6000;
     while(x--);
   }
}

/************************************************************************************************************************
**? ? ?:  	I2C_FRAM_PageWrite(rt_uint8_t* pBuffer, rt_uint16_t WriteAddr, rt_uint16_t NumByteToWrite)            
**????:  	
**??	?:			
**?	?:	???????
**????:
**              2011.11.7??
************************************************************************************************************************/
unsigned char I2C_FRAM_PageWrite(rt_uint8_t* pBuffer, rt_uint16_t WriteAddr, rt_uint16_t NumByteToWrite)
{
        rt_uint8_t FRAM_ADDRESS;
	rt_uint8_t Addr = 0, count = 0;
	Addr = WriteAddr>>8;	    //WriteAddr/256(BLOACKSIZE)
	count = WriteAddr&0x00ff;   //WriteAddr%256(BLOACKSIZE)
	Addr = Addr << 1;
	Addr = Addr & 0x0F;
	FRAM_ADDRESS = I2C1_SLAVE_ADDRESS7 | Addr;
	if (!I2C_Start()) return FALSE;
	I2C_SendByte(FRAM_ADDRESS); //??????+???
	if (!I2C_WaitAck())
	{
		I2C_Stop();
		return FALSE;
	}
	I2C_SendByte(count);        //??????
	if (!I2C_WaitAck())
	{
		I2C_Stop();
		return FALSE;
	}

	while(NumByteToWrite--)
	{
		I2C_SendByte(* pBuffer);
		if (!I2C_WaitAck())
		{
		I2C_Stop();
		return FALSE;
		}
		pBuffer++;
	}
	I2C_Stop();
	I2C_delay_ms(50);
	return TRUE;
}

/************************************************************************************************************************
**? ? ?:  	I2C_FRAM_BufferWrite(rt_uint8_t* pBuffer, rt_uint16_t WriteAddr,rt_uint16_t NumByteToWrite)         
**????:  	
**??	?:			
**?	?:	?????????
**????:
**              2011.11.7??
************************************************************************************************************************/
unsigned char I2C_FRAM_BufferWrite(rt_uint8_t* pBuffer, rt_uint16_t WriteAddr,rt_uint16_t NumByteToWrite)
{
	rt_uint8_t count = 0,PageAligned=0;
	rt_uint16_t finishnum=0;
        PageAligned=WriteAddr%I2C_PageSize;  //???0,??????????????
	count=I2C_PageSize-PageAligned;      //??????????
	if(NumByteToWrite<=count)            //?????,????????
	{
	   if (!I2C_FRAM_PageWrite(pBuffer,WriteAddr,NumByteToWrite))
		{
		  I2C_Stop();
		  return FALSE;
		}
		return TRUE;
	}
	else                                 //????????????,???????
	{
	    if (!I2C_FRAM_PageWrite(pBuffer,WriteAddr,count))
		{
		  I2C_Stop();
		  return FALSE;
		}
		finishnum+=count;
	}
	while(1)
	{
	   if(NumByteToWrite-finishnum>I2C_PageSize)
	   {
	       if (!I2C_FRAM_PageWrite(pBuffer+finishnum,WriteAddr+finishnum,I2C_PageSize))
		   {
				I2C_Stop();
				return FALSE;
		   }
		   finishnum+=I2C_PageSize;
		   continue;
	   }
	   else
	   {
	       if (!I2C_FRAM_PageWrite(pBuffer+finishnum,WriteAddr+finishnum,NumByteToWrite-finishnum))
		   {
				I2C_Stop();
				return FALSE;
		   }
		   finishnum+=NumByteToWrite-finishnum;
		   break;
	   }
	}
	return TRUE;
}

/************************************************************************************************************************
**? ? ?:  	Veryfy(rt_uint8_t* Buffer1,rt_uint8_t* Buffer2,rt_uint16_t len)      
**????:  	
**??	?:			
**?	?:	????
**????:
**              2011.11.7??
************************************************************************************************************************/
unsigned char Veryfy(rt_uint8_t* Buffer1,rt_uint8_t* Buffer2,rt_uint16_t len)
{
  rt_uint16_t i=0;
  for(i=0;i<len;i++)
  {
    if(Buffer1[i]!=Buffer2[i])
	{
	  return FALSE;
	}
  }
  return TRUE;
}

/************************************************************************************************************************
**? ? ?:  	I2C_FRAM_VerifyWrite(rt_uint8_t* pBuffer,rt_uint16_t WriteAddr, rt_uint16_t NumByteToWrite)      
**????:  	
**??	?:			
**?	?:	//????EEROM?,???????EEROM,??3???????????
                //????,?????,??3??????????
**????:
**              2011.11.7??
************************************************************************************************************************/
unsigned char I2C_FRAM_VerifyWrite(rt_uint8_t* pBuffer,rt_uint16_t WriteAddr, rt_uint16_t NumByteToWrite)
{
  rt_uint8_t x=3,VerifyBuffer[100];
  while(x--)
  {
    if(!I2C_FRAM_BufferWrite(pBuffer, WriteAddr, NumByteToWrite))
	{
	  if(x==0)
	  {
	     return FALSE;
	  }
	  else
	  {
	    I2C_delay_ms(100);
	    continue;
	  }
	}
	else
	{
	  break;
	}
  }
  x=3;
  while(x--)
  {
    if(!I2C_FRAM_BufferRead(VerifyBuffer, WriteAddr, NumByteToWrite))
	{
	  if(x==0)
	  {
	     return FALSE;
	  }
	  else
	  {
	    I2C_delay_ms(100);
	    continue;
	  }
	}
	else
	{
	 if(Veryfy(VerifyBuffer,pBuffer,NumByteToWrite)==TRUE)
	 {
	   return TRUE;
	 }
	 else
	 {
	    return FALSE;
	 }
	}
  }
 return   FALSE;
}





unsigned char I2C_FRAM_BufferRead(rt_uint8_t* pBuffer, rt_uint16_t WriteAddr, rt_uint16_t NumByteToRead)
{
        rt_uint8_t FRAM_ADDRESS;
	rt_uint8_t Addr = 0, count = 0;
	Addr = WriteAddr>>8;	    //WriteAddr/256(BLOACKSIZE)
	count = WriteAddr&0x00ff;   //WriteAddr%256(BLOACKSIZE)
	Addr = Addr << 1;
	Addr = Addr & 0x0F;
	FRAM_ADDRESS = I2C1_SLAVE_ADDRESS7 | Addr;
	if (!I2C_Start()) return FALSE;
	I2C_SendByte(FRAM_ADDRESS); //??????+???
	if (!I2C_WaitAck())
	{
	I2C_Stop();
	return FALSE;
	}
	I2C_SendByte(count);        //???????
	I2C_WaitAck();
	I2C_Start();
	I2C_SendByte(FRAM_ADDRESS | 0x01);
	I2C_WaitAck();
	while(NumByteToRead)
	{
	*pBuffer = I2C_ReceiveByte();
	if(NumByteToRead == 1)I2C_NoAck();
	else I2C_Ack();
	pBuffer++;
	NumByteToRead--;
	I2C_delay_ms(20);
	}
	I2C_Stop();
	return TRUE;
}



rt_uint16_t  crc_calculate_crc(rt_uint16_t initial_crc,  rt_uint8_t  *buffer, rt_uint16_t length)
{
	rt_uint16_t index = 0;
	rt_uint16_t crc = initial_crc;
        for (index = 0; index < length; index++)
		{
			crc = (rt_uint16_t)((rt_uint8_t)(crc >> 8) | (rt_uint16_t)(crc << 8));
			crc ^= buffer [index];
			crc ^= (rt_uint8_t)(crc & 0xff) >> 4;
			crc ^= (rt_uint16_t)((rt_uint8_t)(crc << 8) << 4);
			crc ^= (rt_uint16_t)((rt_uint8_t)((crc & 0xff) << 4) << 1);
		}
	return (crc);
}


void rt_hw_I2C_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(SCL_RCC | SDA_RCC,ENABLE);
    
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = SCL_Pin;
    GPIO_Init(SCL_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   =  WP_Pin|SDA_Pin;
    GPIO_Init(SDA_GPIO, &GPIO_InitStructure);
    WP_EN;
    WP_DISEN;
}


const unsigned int RT_Pow10[5] = {
  1 , 10, 100, 1000, 10000
};
void FC_sprintf(float y,char *p,rt_uint8_t shift)
{
     rt_int16_t Stemp,Ftemp;
     if(y<0)
        y=0;
     Stemp=(short)(y);
     if(shift>4)
       shift=4;
     Ftemp=(rt_uint16_t)((y-Stemp)*RT_Pow10[shift]+0.5);
     if(shift==1)
        rt_sprintf(p,"%d.%01d",Stemp,Ftemp);
     else if(shift==2)
        rt_sprintf(p,"%d.%02d",Stemp,Ftemp);
     else if(shift==3)
        rt_sprintf(p,"%d.%03d",Stemp,Ftemp);
     else if(shift==4)
        rt_sprintf(p,"%d.%04d",Stemp,Ftemp);
     else 
        rt_sprintf(p,"%d.%d",Stemp,Ftemp);
}
