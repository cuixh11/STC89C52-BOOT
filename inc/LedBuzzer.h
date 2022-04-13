/*********************************************************************************** 
;功能说明:STC89C52开机次数+数码管温度报警+1602时间
;文件名称:LedBuzzer.h
:描    述：数码管和无源蜂鸣器的驱动模块的头文件
;版本号：v1.0.0  
;微处理器:STC89C52
;编译环境:Keil uVision V4.10 
;作    者:Cui Xinghai 
;创建日期:2022.4.9   
;***********************************************************************************/

#ifndef _LED_BUZZER_H
#define _LED_BUZZER_H

extern     unsigned char dat;

struct sLedBuff {  //LED显示缓冲区结构
    uint8 array[8];   //点阵缓冲区
    uint8 number[6];  //数码管缓冲区
    uint8 alone;      //独立LED缓冲区
};


#ifndef _LED_BUZZER_C

#endif


void LedScan();
void BuzzBeep();
void BuzzCtrl();

#endif
