/*********************************************************************************** 
;功能说明:STC89C52开机次数+数码管温度报警+1602时间
;文件名称:I2C.h
:描    述：I2C总线驱动模块的头文件	* 备注：基于IO口模拟实现，总线时序延时等皆由软件方式实现
;版本号：v1.0.0  
;微处理器:STC89C52
;编译环境:Keil uVision V4.10 
;作    者:Cui Xinghai 
;创建日期:2022.4.9   
;***********************************************************************************/

#ifndef _I2C_H
#define _I2C_H


#ifndef _I2C_C

#endif

void I2CStart();
void I2CStop();
uint8 I2CReadACK();
uint8 I2CReadNAK();
bit I2CWrite(uint8 dat);



#endif
