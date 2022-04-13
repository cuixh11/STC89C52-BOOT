/*********************************************************************************** 
;功能说明:STC89C52开机次数+数码管温度报警+1602时间
;文件名称:LedBuzzer.c
:描    述：数码管和无源蜂鸣器的驱动模块
;版本号：v1.0.0  									   
;编译环境:Keil uVision V4.10 
;微处理器:STC89C52
;作    者:Cui Xinghai 
;创建日期:2022.4.9   
;***********************************************************************************/

#define  _LED_BUZZER_C
#include "config.h"
#include "LedBuzzer.h"

extern int16 temp;
 struct sLedBuff ledBuff; //LED显示缓冲区，默认初值全0，正好达到上电全亮的效果
unsigned char code LedChar[] = {  //数码管显示字符转换表
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E,0xFF
};
uint8 staBuzzer = 0; //蜂鸣器状态控制，非0-鸣叫、0-关闭

/* LED动态显示扫描，需在定时中断中调用 */
void LedScan()
{
    static uint8 i = 0;  //LED位选索引
    
    if (ENLED == 0)  //LED使能时才进行动态扫描
    {
        P0 = 0xFF;                       //关闭所有段选位，显示消隐
        P1 = (P1 & 0xF0) | i;            //位选索引值赋值到P1口低4位
        P0 = *((uint8 data*)&ledBuff+i); //缓冲区中索引位置的数据送到P0口
        if (i < (sizeof(ledBuff)-1))     //索引递增循环，遍历整个缓冲区
            i++;
        else
            i = 0;
    }
}

/* 蜂鸣器“嘀”一声 */
void BuzzBeep()
{
    staBuzzer = 80;			//1000 0000
}

/* 蜂鸣器发声控制，需在定时中断中调用 */
void BuzzCtrl()
{

    if (staBuzzer != 0)
    {
        BUZZER = ~BUZZER;  //蜂鸣器鸣叫
        staBuzzer--;
    }
    else
    {
        BUZZER = 1;        //蜂鸣器静音
    }
}
