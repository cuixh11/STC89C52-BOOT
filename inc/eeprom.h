/*********************************************************************************** 
;功能说明:STC89C52开机次数+数码管温度报警+1602时间
;文件名称:eeprom.h
:描    述：EEPROM单字节读写操作,每次+1后写回
;版本号：v1.0.0  
;微处理器:STC89C52
;编译环境:Keil uVision V4.10 
;作    者:Cui Xinghai 
;创建日期:2022.4.9   
;***********************************************************************************/

#ifndef _EEPROM_H
#define _EEPROM_H


#ifndef _EEPROM_C

#endif

unsigned char E2ReadByte(unsigned char addr);
void E2WriteByte(unsigned char addr, unsigned char dat);


#endif
