/*********************************************************************************** 
;����˵��:STC89C52��������+������¶ȱ���+1602ʱ��
;�ļ�����:eeprom.c
:��    ����EEPROM���ֽڶ�д����,ÿ��+1��д��
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.9   
;***********************************************************************************/

#include <reg52.h>
#include "config.h"
#include "I2C.h"


/* ��ȡEEPROM�е�һ���ֽڣ�addr-�ֽڵ�ַ */
unsigned char E2ReadByte(unsigned char addr)
{
    unsigned char dat;
    
    I2CStart();
    I2CWrite(0x50<<1); //Ѱַ����������Ϊд����
    I2CWrite(addr);    //д��洢��ַ
    I2CStart();        //�����ظ������ź�
    I2CWrite((0x50<<1)|0x01); //Ѱַ����������Ϊ������
    dat = I2CReadNAK();       //��ȡһ���ֽ�����
    I2CStop();
    
    return dat;
}
/* ��EEPROM��д��һ���ֽڣ�addr-�ֽڵ�ַ */
void E2WriteByte(unsigned char addr, unsigned char dat)
{
    I2CStart();
    I2CWrite(0x50<<1); //Ѱַ����������Ϊд����
    I2CWrite(addr);    //д��洢��ַ
    I2CWrite(dat);     //д��һ���ֽ�����
    I2CStop();
}
