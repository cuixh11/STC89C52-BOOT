/*********************************************************************************** 
;功能说明:STC89C52开机次数+数码管温度报警+1602时间
;文件名称:Lcd1602.h
:描    述：1602字符液晶驱动模块的头文件   
;版本号：v1.0.0  
;微处理器:STC89C52
;编译环境:Keil uVision V4.10 
;作    者:Cui Xinghai 
;创建日期:2022.4.9   
;***********************************************************************************/

#ifndef _LCD1602_H
#define _LCD1602_H


#ifndef _LCD1602_C

#endif

void LcdWaitReady();
void LcdWriteCmd(unsigned char cmd);
void LcdSetCursor(unsigned char x, unsigned char y);
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
void InitLcd1602();


#endif
