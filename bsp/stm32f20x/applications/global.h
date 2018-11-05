#ifndef __GLOBAL_H
#define __GLOBAL_H
#include "stm32f2xx.h"
#include <rtthread.h>
///////////////////////////////////////////////////////////////////
//#define S0_ctrl  2
//#define S1_ctrl  3
//#define S2_ctrl  0
//#define S3_ctrl  1


typedef struct     
{
  rt_uint8_t    SF_SEL;                             //标频选择  内外
	rt_uint16_t   GateTime;                         //参考闸门时间 单位mS 
  rt_uint32_t   StdFreq;                            //标频值    单位HZ
}FreqMeas_Para;      //保存控温参数






#endif
