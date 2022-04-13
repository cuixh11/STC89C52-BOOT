/*********************************************************************************** 
;功能说明:STC89C52开机次数+数码管温度报警+1602时间
;文件名称:main.c     
;版本号：v1.0.0  
;微处理器:STC89C52
;编译环境:Keil uVision V4.10 
;作    者:Cui Xinghai 
;创建日期:2022.4.9   
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

bit 	flag100ms	 = 0; 	 //100ms定时标志
uint8 	T0RH		 = 0;    //T0重载值的高字节
uint8 	T0RL 		 = 0;    //T0重载值的低字节
int 	tempalarm 	 = 26;	 //温度蜂鸣器报警初始值
uint8	dat;

extern struct sLedBuff ledBuff; //LED显示缓冲区，默认初值全0，正好达到上电全亮的效果
extern unsigned char code LedChar[]; //数码管显示字符转换表

void main()
{
	 uint8 tmr1s = 10;
	 uint8 delay = 1;

    EA = 1;       //使能总中断
	ConfigTimer0(1);  //配置T0定时1ms
    ENLED = 0;    //使能U3，选择控制数码管
    ADDR3 = 1;    //因为需要动态改变ADDR0-2的值，所以不需要再初始化了

	dat =E2ReadByte(0x02);    //读取指定地址上的一个字节
	LedNumber();
	if (dat>98) dat =0;
	else ++dat;
	E2WriteByte(0x02, dat);    //再写回到对应的地址上
	
    InitLcd1602();    //初始化液晶模块
	Start18B20();     //启动首次温度转换
	BuzzBeep();       //蜂鸣器提示

    while (1)
	{
	 	KeyDriver();      //执行按键驱动
		if (flag100ms)    //每隔100ms执行以下分支
        {
            flag100ms = 0;
			if (delay > 0)
            {
                delay--;
                if (delay == 0)
                {   //本段代码仅在上电延时后执行一次
                    InitDS1302();  //初始化实时时钟模块
                }
                continue;
            }
            ShowRealTime();  //时间显示刷新
            tmr1s++;         //每隔1s执行以下分支			        
			if (tmr1s >= 10)
			{
				tmr1s = 0;
				RefreshTemp();	 //温度刷新显示	 																																																																	
			}	
		}
	}
}
/* 开机次数数码管显示 */
void LedNumber()
{			
    //以下代码将sec按十进制位从低到高依次提取并转为数码管显示字符
    ledBuff.number[3] = LedChar[16];
    ledBuff.number[4] = LedChar[dat%10];
    ledBuff.number[5] = LedChar[dat/10%10];
}
/* 温度刷新函数，读取当前温度并根据需要刷新液晶显示 */
void RefreshTemp()
{
    int16 temp;
    int intT, decT;  //温度值的整数和小数部分

    static int16 backup = 32767;  //保证首次执行时必定刷新
    
    Get18B20Temp(&temp); //获取当前温度值
    Start18B20();        //启动下一次转换

    intT = temp >> 4;             //分离出温度值整数部分
    decT = temp & 0xF;            //0x0000 0000 0000 1111分离出温度值小数部分
    temp_alarm(&intT);

	if (backup != temp)  //温度变化时刷新液晶显示
    {
        ledBuff.number[2] = LedChar[intT/10] ;  //十位转为ASCII码
        ledBuff.number[1] = LedChar[intT%10] & 0x7f ;  //个位转为ASCII码
        ledBuff.number[0] = LedChar[(decT*10) / 16];             
        backup = temp;             //刷新上次温度值
	}
}
/*温度蜂鸣器报警指示*/
void temp_alarm( int *TEMP) 
{  
	  if(*TEMP >= tempalarm)
	  {
	    BuzzBeep();
	  }
	  else 
	  {
	    BUZZER = 1;        //蜂鸣器静音
	  }
}                  
/* 将一个字节按十六进制格式显示到屏幕上，(x,y)-屏幕起始坐标，byt-待显示字节 */
void ShowHexByte(uint8 x, uint8 y, uint8 byt)
{
    uint8 tmp;
    uint8 str[4];
    
    //高四位转为十六进制字符
    tmp = byt >> 4;
    if (tmp <= 9)
        str[0] = tmp + '0';
    else
        str[0] = tmp - 10 + 'A';
    //低四位转为十六进制字符
    tmp = byt & 0x0F;
    if (tmp <= 9)
        str[1] = tmp + '0';
    else
        str[1] = tmp - 10 + 'A';
    //添加结束符并显示到液晶
    str[2] = '\0';
    LcdShowStr(x, y, str);
}
/* 读取DS1302时间，显示在液晶屏上 */
void ShowRealTime()
{
    struct sTime buf;
    static struct sTime pdata bkp = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

    GetRealTime(&buf);
	if (bkp.year != buf.year)  //年变化时刷新显示
    {
        ShowHexByte(1, 0, buf.year);
        bkp.year = buf.year;
    }
	if (bkp.mon != buf.mon)  //月变化时刷新显示
    {
        ShowHexByte(0, 1, buf.mon);
        bkp.mon = buf.mon;
    }
	if (bkp.day != buf.day)  //月变化时刷新显示
    {
        ShowHexByte(3, 1, buf.day);
        bkp.day = buf.day;
    }
    if (bkp.hour != buf.hour)  //小时变化时刷新显示
    {
        ShowHexByte(6, 1, buf.hour);
        bkp.hour = buf.hour;
    }
    if (bkp.min != buf.min)  //分钟变化时刷新显示
    {
        ShowHexByte(9, 1, buf.min);
        bkp.min = buf.min;
    }
    if (bkp.sec != buf.sec)  //秒变化时刷新显示
    {
        ShowHexByte(12, 1, buf.sec);
        bkp.sec = buf.sec;
    }
}
/* 配置并启动T0，ms-T0定时时间 */
void ConfigTimer0(uint16 ms)
{
    uint32 tmp;
    
    tmp = (SYS_MCLK*ms)/1000; //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 33;           //补偿中断响应延时造成的误差   
    T0RH = (uint8)(tmp>>8);   //定时器重载值拆分为高低字节
    T0RL = (uint8)tmp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}
/* T0中断服务函数，实现系统定时和按键扫描 */
void InterruptTimer0() interrupt 1
{
    static uint8 tmr100ms = 0;
    
    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;

    BuzzCtrl();  //蜂鸣器发声控制
    LedScan();   //执行LED扫描
//    KeyScan();   //执行按键扫描

    tmr100ms++;  //定时100ms
    if (tmr100ms >= 100)
    {
        tmr100ms = 0;
        flag100ms = 1;
    }
}

