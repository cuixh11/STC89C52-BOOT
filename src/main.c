/*********************************************************************************** 
;����˵��:STC89C52��������+������¶ȱ���+1602ʱ��
;�ļ�����:main.c     
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.9   
;***********************************************************************************/

#define  _MAIN_C
#include <REG52.H>

#include "config.h"
#include "LCD1602.h"
#include "LedBuzzer.h"
#include "I2C.h"
#include "eeprom.h"
#include "keyboard.h"
#include "DS18B20.h"
#include "DS1302.h"
#include "main.h"

#include <reg52.h>

bit 	flag100ms	 = 0; 	 //100ms��ʱ��־
uint8 	T0RH		 = 0;    //T0����ֵ�ĸ��ֽ�
uint8 	T0RL 		 = 0;    //T0����ֵ�ĵ��ֽ�
int 	tempalarm 	 = 26;	 //�¶ȷ�����������ʼֵ
uint8	dat;

extern struct sLedBuff ledBuff; //LED��ʾ��������Ĭ�ϳ�ֵȫ0�����ôﵽ�ϵ�ȫ����Ч��
extern unsigned char code LedChar[]; //�������ʾ�ַ�ת����

void main()
{
	 uint8 tmr1s = 10;
	 uint8 delay = 1;

    EA = 1;       //ʹ�����ж�
	ConfigTimer0(1);  //����T0��ʱ1ms
    ENLED = 0;    //ʹ��U3��ѡ����������
    ADDR3 = 1;    //��Ϊ��Ҫ��̬�ı�ADDR0-2��ֵ�����Բ���Ҫ�ٳ�ʼ����

	dat =E2ReadByte(0x02);    //��ȡָ����ַ�ϵ�һ���ֽ�
	LedNumber();
	if (dat>98) dat =0;
	else ++dat;
	E2WriteByte(0x02, dat);    //��д�ص���Ӧ�ĵ�ַ��
	
    InitLcd1602();    //��ʼ��Һ��ģ��
	Start18B20();     //�����״��¶�ת��
	BuzzBeep();       //��������ʾ

    while (1)
	{
	 	KeyDriver();      //ִ�а�������
		if (flag100ms)    //ÿ��100msִ�����·�֧
        {
            flag100ms = 0;
			if (delay > 0)
            {
                delay--;
                if (delay == 0)
                {   //���δ�������ϵ���ʱ��ִ��һ��
                    InitDS1302();  //��ʼ��ʵʱʱ��ģ��
                }
                continue;
            }
            ShowRealTime();  //ʱ����ʾˢ��
            tmr1s++;         //ÿ��1sִ�����·�֧			        
			if (tmr1s >= 10)
			{
				tmr1s = 0;
				RefreshTemp();	 //�¶�ˢ����ʾ	 																																																																	
			}	
		}
	}
}
/* ���������������ʾ */
void LedNumber()
{			
    //���´��뽫sec��ʮ����λ�ӵ͵���������ȡ��תΪ�������ʾ�ַ�
    ledBuff.number[3] = LedChar[16];
    ledBuff.number[4] = LedChar[dat%10];
    ledBuff.number[5] = LedChar[dat/10%10];
}
/* �¶�ˢ�º�������ȡ��ǰ�¶Ȳ�������Ҫˢ��Һ����ʾ */
void RefreshTemp()
{
    int16 temp;
    int intT, decT;  //�¶�ֵ��������С������

    static int16 backup = 32767;  //��֤�״�ִ��ʱ�ض�ˢ��
    
    Get18B20Temp(&temp); //��ȡ��ǰ�¶�ֵ
    Start18B20();        //������һ��ת��

    intT = temp >> 4;             //������¶�ֵ��������
    decT = temp & 0xF;            //0x0000 0000 0000 1111������¶�ֵС������
    temp_alarm(&intT);

	if (backup != temp)  //�¶ȱ仯ʱˢ��Һ����ʾ
    {
        ledBuff.number[2] = LedChar[intT/10] ;  //ʮλתΪASCII��
        ledBuff.number[1] = LedChar[intT%10] & 0x7f ;  //��λתΪASCII��
        ledBuff.number[0] = LedChar[(decT*10) / 16];             
        backup = temp;             //ˢ���ϴ��¶�ֵ
	}
}
/*�¶ȷ���������ָʾ*/
void temp_alarm( int *TEMP) 
{  
	  if(*TEMP >= tempalarm)
	  {
	    BuzzBeep();
	  }
	  else 
	  {
	    BUZZER = 1;        //����������
	  }
}                  
/* ��һ���ֽڰ�ʮ�����Ƹ�ʽ��ʾ����Ļ�ϣ�(x,y)-��Ļ��ʼ���꣬byt-����ʾ�ֽ� */
void ShowHexByte(uint8 x, uint8 y, uint8 byt)
{
    uint8 tmp;
    uint8 str[4];
    
    //����λתΪʮ�������ַ�
    tmp = byt >> 4;
    if (tmp <= 9)
        str[0] = tmp + '0';
    else
        str[0] = tmp - 10 + 'A';
    //����λתΪʮ�������ַ�
    tmp = byt & 0x0F;
    if (tmp <= 9)
        str[1] = tmp + '0';
    else
        str[1] = tmp - 10 + 'A';
    //��ӽ���������ʾ��Һ��
    str[2] = '\0';
    LcdShowStr(x, y, str);
}
/* ��ȡDS1302ʱ�䣬��ʾ��Һ������ */
void ShowRealTime()
{
    struct sTime buf;
    static struct sTime pdata bkp = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

    GetRealTime(&buf);
	if (bkp.year != buf.year)  //��仯ʱˢ����ʾ
    {
        ShowHexByte(1, 0, buf.year);
        bkp.year = buf.year;
    }
	if (bkp.mon != buf.mon)  //�±仯ʱˢ����ʾ
    {
        ShowHexByte(0, 1, buf.mon);
        bkp.mon = buf.mon;
    }
	if (bkp.day != buf.day)  //�±仯ʱˢ����ʾ
    {
        ShowHexByte(3, 1, buf.day);
        bkp.day = buf.day;
    }
    if (bkp.hour != buf.hour)  //Сʱ�仯ʱˢ����ʾ
    {
        ShowHexByte(6, 1, buf.hour);
        bkp.hour = buf.hour;
    }
    if (bkp.min != buf.min)  //���ӱ仯ʱˢ����ʾ
    {
        ShowHexByte(9, 1, buf.min);
        bkp.min = buf.min;
    }
    if (bkp.sec != buf.sec)  //��仯ʱˢ����ʾ
    {
        ShowHexByte(12, 1, buf.sec);
        bkp.sec = buf.sec;
    }
}
/* ���ò�����T0��ms-T0��ʱʱ�� */
void ConfigTimer0(uint16 ms)
{
    uint32 tmp;
    
    tmp = (SYS_MCLK*ms)/1000; //��������ļ���ֵ
    tmp = 65536 - tmp;        //���㶨ʱ������ֵ
    tmp = tmp + 33;           //�����ж���Ӧ��ʱ��ɵ����   
    T0RH = (uint8)(tmp>>8);   //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (uint8)tmp;
    TMOD &= 0xF0;   //����T0�Ŀ���λ
    TMOD |= 0x01;   //����T0Ϊģʽ1
    TH0 = T0RH;     //����T0����ֵ
    TL0 = T0RL;
    ET0 = 1;        //ʹ��T0�ж�
    TR0 = 1;        //����T0
}
/* T0�жϷ�������ʵ��ϵͳ��ʱ�Ͱ���ɨ�� */
void InterruptTimer0() interrupt 1
{
    static uint8 tmr100ms = 0;
    
    TH0 = T0RH;  //���¼�������ֵ
    TL0 = T0RL;

    BuzzCtrl();  //��������������
    LedScan();   //ִ��LEDɨ��
//    KeyScan();   //ִ�а���ɨ��

    tmr100ms++;  //��ʱ100ms
    if (tmr100ms >= 100)
    {
        tmr100ms = 0;
        flag100ms = 1;
    }
}

