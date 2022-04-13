/*********************************************************************************** 
;����˵��:STC89C52��������+������¶ȱ���+1602ʱ��
;�ļ�����:I2C.c
:��    ����I2C��������ģ��	* ��ע������IO��ģ��ʵ�֣�����ʱ����ʱ�Ƚ���������ʽʵ��
;�汾�ţ�v1.0.0  
;΢������:STC89C52
;���뻷��:Keil uVision V4.10 
;��    ��:Cui Xinghai 
;��������:2022.4.9   
;***********************************************************************************/

#define  _I2C_C
#include "config.h"
#include "I2C.h"

#define I2CDelay()  {_nop_();_nop_();_nop_();_nop_();}

/* ����������ʼ�ź� */
void I2CStart()
{
    I2C_SDA = 1; //����ȷ��SDA��SCL���Ǹߵ�ƽ
    I2C_SCL = 1;
    I2CDelay();
    I2C_SDA = 0; //������SDA
    I2CDelay();
    I2C_SCL = 0; //������SCL
}
/* ��������ֹͣ�ź� */
void I2CStop()
{
    I2C_SCL = 0; //����ȷ��SDA��SCL���ǵ͵�ƽ
    I2C_SDA = 0;
    I2CDelay();
    I2C_SCL = 1; //������SCL
    I2CDelay();
    I2C_SDA = 1; //������SDA
    I2CDelay();
}
/* I2C����д������dat-��д���ֽڣ�����ֵ-�ӻ�Ӧ��λ��ֵ */
bit I2CWrite(uint8 dat)
{
    bit ack;  //�����ݴ�Ӧ��λ��ֵ
    uint8 mask;  //����̽���ֽ���ĳһλֵ���������

    for (mask=0x80; mask!=0; mask>>=1) //�Ӹ�λ����λ���ν���
    {
        if ((mask&dat) == 0)  //��λ��ֵ�����SDA��
            I2C_SDA = 0;
        else
            I2C_SDA = 1;
        I2CDelay();
        I2C_SCL = 1;          //����SCL
        I2CDelay();
        I2C_SCL = 0;          //������SCL�����һ��λ����
    }
    I2C_SDA = 1;   //8λ���ݷ�����������ͷ�SDA���Լ��ӻ�Ӧ��
    I2CDelay();
    I2C_SCL = 1;   //����SCL
    ack = I2C_SDA; //��ȡ��ʱ��SDAֵ����Ϊ�ӻ���Ӧ��ֵ
    I2CDelay();
    I2C_SCL = 0;   //������SCL���Ӧ��λ��������ס����

    return (~ack); //Ӧ��ֵȡ���Է���ͨ�����߼���
                   //0=�����ڻ�æ��д��ʧ�ܣ�1=�����ҿ��л�д��ɹ�
}
/* I2C���߶������������ͷ�Ӧ���źţ�����ֵ-�������ֽ� */
uint8 I2CReadNAK()
{
    uint8 mask;
    uint8 dat;

    I2C_SDA = 1;  //����ȷ�������ͷ�SDA
    for (mask=0x80; mask!=0; mask>>=1) //�Ӹ�λ����λ���ν���
    {
        I2CDelay();
        I2C_SCL = 1;      //����SCL
        if(I2C_SDA == 0)  //��ȡSDA��ֵ
            dat &= ~mask; //Ϊ0ʱ��dat�ж�Ӧλ����
        else
            dat |= mask;  //Ϊ1ʱ��dat�ж�Ӧλ��1
        I2CDelay();
        I2C_SCL = 0;      //������SCL����ʹ�ӻ����ͳ���һλ
    }
    I2C_SDA = 1;   //8λ���ݷ����������SDA�����ͷ�Ӧ���ź�
    I2CDelay();
    I2C_SCL = 1;   //����SCL
    I2CDelay();
    I2C_SCL = 0;   //������SCL��ɷ�Ӧ��λ��������ס����

    return dat;
}
/* I2C���߶�������������Ӧ���źţ�����ֵ-�������ֽ� */
uint8 I2CReadACK()
{
    uint8 mask;
    uint8 dat;

    I2C_SDA = 1;  //����ȷ�������ͷ�SDA
    for (mask=0x80; mask!=0; mask>>=1) //�Ӹ�λ����λ���ν���
    {
        I2CDelay();
        I2C_SCL = 1;      //����SCL
        if(I2C_SDA == 0)  //��ȡSDA��ֵ
            dat &= ~mask; //Ϊ0ʱ��dat�ж�Ӧλ����
        else
            dat |= mask;  //Ϊ1ʱ��dat�ж�Ӧλ��1
        I2CDelay();
        I2C_SCL = 0;      //������SCL����ʹ�ӻ����ͳ���һλ
    }
    I2C_SDA = 0;   //8λ���ݷ����������SDA������Ӧ���ź�
    I2CDelay();
    I2C_SCL = 1;   //����SCL
    I2CDelay();
    I2C_SCL = 0;   //������SCL���Ӧ��λ��������ס����

    return dat;
}