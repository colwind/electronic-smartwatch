/*******************************************************************************
***********************基于MSP430单片机的无线智能手表***************************
********************************************************************************

*主要硬件列表：MSP430F1611 （16位超低功耗单片机）
               MicroSD Card *1 （FAT文件系统）
               Eink Screen *1（2.04寸/172*72/大连佳显）
               nRF24L01 *1 （2.4G全双工无线芯片/模块）

***********************************************demonfs@my.swjtu.edu.cn*********/
/********************************全局变量定义**********************************/


typedef struct TIME//全局时间结构体
{
 unsigned int year;
 unsigned char month;
 unsigned char day;
 unsigned char weekday;
 unsigned char h;
 unsigned char m;
 unsigned char s;
} TIME;



TIME time;
TIME alarm;
unsigned char ClocK_Mode=1;//钟表模式。1表示开启
unsigned char nRF_ON=0x00;//RF开启标志0:开启RF 1：RF数据等待处理 2.码表打开了
unsigned char TIME_FLAG=0x00;//0分钟触发 1闰年，2十二小时制 3闹钟开关 4整点报时
unsigned char battery_Flag=0x00;//电池电量，分4个等级，用低四位，高四位4正在充电
                                //5充电完成
/*****************************库文档与子程序文档*******************************/
#include <msp430x16x.h>
#include "BOARD_CONFIG.c"
#include "SD.c"
#include "EINK.c"
#include "RF.c"
#include "TIME.c"
#include "BUZ.c"
#include "KEY.c"




/*******************************************************************************
*函数名称：LED_IO_set
*函数功能：最终测试版v1.0上面的提示LED
*说明：在P45上面，拉电流
*******************************************************************************/
void LED_IO_set(void)
{    
  P4DIR |= BIT5;
  P4SEL &= ~BIT5;
}
/*********************************P1口上的中断*********************************/
#pragma vector=PORT1_VECTOR
__interrupt void  PORT1_ISR(void)
{
  
  if(P1IFG&Key_backBIT)
  {
    Key_int_ban(1);
    Key_flag=0;
    P1IFG = 0x00;
    __low_power_mode_off_on_exit();
    return;
  }
  if(P1IFG&Key_L1BIT)
  {
    Key_int_ban(1);
    Key_flag=2;
    P1IFG = 0x00;
    __low_power_mode_off_on_exit();
    return;
  }
  if(P1IFG&Key_R2BIT)
  {
    Key_int_ban(1);
    Key_flag=3;
    P1IFG = 0x00;
    __low_power_mode_off_on_exit();
    return;
  }
    if(P1IFG & BIT3)//nRF24L01的IRQ中断
    {
       if(nRF_ON&BIT0)
       {
           nRF_ON |= BIT1;
       }
    }
    P1IFG = 0x00;
    return;
}
/*********************************P2口上的中断*********************************/
#pragma vector=PORT2_VECTOR
__interrupt void  PORT2_ISR(void)
{
    if(P2IFG&Key_enter3BIT)
  {
    Key_int_ban(1);
    Key_flag=3;
    P2IFG = 0x00;
    return;
  }
  
  P2IFG = 0x00;
  return;
}
/******************************************************************************/
/********************************看门狗中断************************************/
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    Processing_Time();
    if((TIME_FLAG&BIT0)&&(ClocK_Mode==1))
    {
      __low_power_mode_off_on_exit();//退出低功耗模式
    }
    return;
}


/******************************************************************************/
void main( void )
{
    //单片机初始化
    WDT_Init();
    Clock_Init();
    //引脚初始化
    EInk_IO_init();
    LED_IO_set();
    RF24L01_IO_set(); 
    init_key_port();
    
    //功能模块初始化
    INIT_SD_Card();
    init_NRF24L01() ;
    _EINT();
    
    Time_init();
    //SetRX_Mode();
    while(1)
    {
        //OUTPUT_TIME_SCREEN();
        //OUTPUT_IMAGE_FILE("TEST0001TXT",1);
        OUTPUT_S_TIME("MENU1001TXT");
       // mmc_GoIdle();
        OUTPUT_S_TIME("MENU2001TXT");
       // mmc_GoIdle();
        OUTPUT_S_TIME("MENU2002TXT");
       // mmc_GoIdle();
        OUTPUT_S_TIME("MENU2031TXT");
       // mmc_GoIdle();
        OUTPUT_S_TIME("MENU2004TXT");
       // mmc_GoIdle();
        
    
        //======时钟部分============================================================
        if(TIME_FLAG&BIT0)
        {
            OUTPUT_TIME_SCREEN();
            TIME_FLAG &= ~BIT0;
        }
        //======按键部分===============================================================
        //======RF部分==============================================================
        if(nRF_ON&BIT1)
        {
            if(nRF24L01_RxPacket(RxBuf))
            {
                OUTPUT_IMAGE_FILE("TEST0001TXT",1);
            }
            nRF_ON &= ~BIT1;
        }
        //==========================================================================


        mmc_GoIdle();              //设置MMC/SD卡在Idle模式，请取出卡到电脑上查看内容
        __low_power_mode_3();
        LPM3;
    }

}
