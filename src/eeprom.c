/*********************************************************************************** 
;功能说明:STC89C52开机次数+数码管温度报警+1602时间
;文件名称:eeprom.c
:描    述：EEPROM单字节读写操作,每次+1后写回
;版本号：v1.0.0  
;微处理器:STC89C52
;编译环境:Keil uVision V4.10 
;作    者:Cui Xinghai 
;创建日期:2022.4.9   
;***********************************************************************************/

#include <reg52.h>
#include "config.h"
#include "I2C.h"


/* 读取EEPROM中的一个字节，addr-字节地址 */
unsigned char E2ReadByte(unsigned char addr)
{
    unsigned char dat;
    
    I2CStart();
    I2CWrite(0x50<<1); //寻址器件，后续为写操作
    I2CWrite(addr);    //写入存储地址
    I2CStart();        //发送重复启动信号
    I2CWrite((0x50<<1)|0x01); //寻址器件，后续为读操作
    dat = I2CReadNAK();       //读取一个字节数据
    I2CStop();
    
    return dat;
}
/* 向EEPROM中写入一个字节，addr-字节地址 */
void E2WriteByte(unsigned char addr, unsigned char dat)
{
    I2CStart();
    I2CWrite(0x50<<1); //寻址器件，后续为写操作
    I2CWrite(addr);    //写入存储地址
    I2CWrite(dat);     //写入一个字节数据
    I2CStop();
}
