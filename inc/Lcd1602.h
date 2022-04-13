/*********************************************************************************** 
;����˵��:STC89C52��������+������¶ȱ���+1602ʱ��
;�ļ�����:Lcd1602.h
:��    ����1602�ַ�Һ������ģ���ͷ�ļ�   
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.9   
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
