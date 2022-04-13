/*********************************************************************************** 
;����˵��:STC89C52��������+������¶ȱ���+1602ʱ��
;�ļ�����:LedBuzzer.h
:��    ��������ܺ���Դ������������ģ���ͷ�ļ�
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.9   
;***********************************************************************************/

#ifndef _LED_BUZZER_H
#define _LED_BUZZER_H

extern     unsigned char dat;

struct sLedBuff {  //LED��ʾ�������ṹ
    uint8 array[8];   //���󻺳���
    uint8 number[6];  //����ܻ�����
    uint8 alone;      //����LED������
};


#ifndef _LED_BUZZER_C

#endif


void LedScan();
void BuzzBeep();
void BuzzCtrl();

#endif
