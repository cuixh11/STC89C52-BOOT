/*********************************************************************************** 
;����˵��:STC89C52��������+������¶ȱ���+1602ʱ��
;�ļ�����:eeprom.h
:��    ����EEPROM���ֽڶ�д����,ÿ��+1��д��
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.9   
;***********************************************************************************/

#ifndef _EEPROM_H
#define _EEPROM_H


#ifndef _EEPROM_C

#endif

unsigned char E2ReadByte(unsigned char addr);
void E2WriteByte(unsigned char addr, unsigned char dat);


#endif
