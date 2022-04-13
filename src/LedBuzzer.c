/*********************************************************************************** 
;����˵��:STC89C52��������+������¶ȱ���+1602ʱ��
;�ļ�����:LedBuzzer.c
:��    ��������ܺ���Դ������������ģ��
;�汾�ţ�v1.0.0  									   
;���뻷��:Keil uVision V4.10 
;΢������:STC89C52
;��    ��:Cui Xinghai 
;��������:2022.4.9   
;***********************************************************************************/

#define  _LED_BUZZER_C
#include "config.h"
#include "LedBuzzer.h"

extern int16 temp;
 struct sLedBuff ledBuff; //LED��ʾ��������Ĭ�ϳ�ֵȫ0�����ôﵽ�ϵ�ȫ����Ч��
unsigned char code LedChar[] = {  //�������ʾ�ַ�ת����
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E,0xFF
};
uint8 staBuzzer = 0; //������״̬���ƣ���0-���С�0-�ر�

/* LED��̬��ʾɨ�裬���ڶ�ʱ�ж��е��� */
void LedScan()
{
    static uint8 i = 0;  //LEDλѡ����
    
    if (ENLED == 0)  //LEDʹ��ʱ�Ž��ж�̬ɨ��
    {
        P0 = 0xFF;                       //�ر����ж�ѡλ����ʾ����
        P1 = (P1 & 0xF0) | i;            //λѡ����ֵ��ֵ��P1�ڵ�4λ
        P0 = *((uint8 data*)&ledBuff+i); //������������λ�õ������͵�P0��
        if (i < (sizeof(ledBuff)-1))     //��������ѭ������������������
            i++;
        else
            i = 0;
    }
}

/* ���������֡�һ�� */
void BuzzBeep()
{
    staBuzzer = 80;			//1000 0000
}

/* �������������ƣ����ڶ�ʱ�ж��е��� */
void BuzzCtrl()
{

    if (staBuzzer != 0)
    {
        BUZZER = ~BUZZER;  //����������
        staBuzzer--;
    }
    else
    {
        BUZZER = 1;        //����������
    }
}
