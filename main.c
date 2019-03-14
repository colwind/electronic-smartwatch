/*******************************************************************************
***********************����MSP430��Ƭ�������������ֱ�***************************
********************************************************************************

*��ҪӲ���б�MSP430F1611 ��16λ���͹��ĵ�Ƭ����
               MicroSD Card *1 ��FAT�ļ�ϵͳ��
               Eink Screen *1��2.04��/172*72/�������ԣ�
               nRF24L01 *1 ��2.4Gȫ˫������оƬ/ģ�飩

***********************************************demonfs@my.swjtu.edu.cn*********/
/********************************ȫ�ֱ�������**********************************/


typedef struct TIME//ȫ��ʱ��ṹ��
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
unsigned char ClocK_Mode=1;//�ӱ�ģʽ��1��ʾ����
unsigned char nRF_ON=0x00;//RF������־0:����RF 1��RF���ݵȴ����� 2.������
unsigned char TIME_FLAG=0x00;//0���Ӵ��� 1���꣬2ʮ��Сʱ�� 3���ӿ��� 4���㱨ʱ
unsigned char battery_Flag=0x00;//��ص�������4���ȼ����õ���λ������λ4���ڳ��
                                //5������
/*****************************���ĵ����ӳ����ĵ�*******************************/
#include <msp430x16x.h>
#include "BOARD_CONFIG.c"
#include "SD.c"
#include "EINK.c"
#include "RF.c"
#include "TIME.c"
#include "BUZ.c"
#include "KEY.c"




/*******************************************************************************
*�������ƣ�LED_IO_set
*�������ܣ����ղ��԰�v1.0�������ʾLED
*˵������P45���棬������
*******************************************************************************/
void LED_IO_set(void)
{    
  P4DIR |= BIT5;
  P4SEL &= ~BIT5;
}
/*********************************P1���ϵ��ж�*********************************/
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
    if(P1IFG & BIT3)//nRF24L01��IRQ�ж�
    {
       if(nRF_ON&BIT0)
       {
           nRF_ON |= BIT1;
       }
    }
    P1IFG = 0x00;
    return;
}
/*********************************P2���ϵ��ж�*********************************/
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
/********************************���Ź��ж�************************************/
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    Processing_Time();
    if((TIME_FLAG&BIT0)&&(ClocK_Mode==1))
    {
      __low_power_mode_off_on_exit();//�˳��͹���ģʽ
    }
    return;
}


/******************************************************************************/
void main( void )
{
    //��Ƭ����ʼ��
    WDT_Init();
    Clock_Init();
    //���ų�ʼ��
    EInk_IO_init();
    LED_IO_set();
    RF24L01_IO_set(); 
    init_key_port();
    
    //����ģ���ʼ��
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
        
    
        //======ʱ�Ӳ���============================================================
        if(TIME_FLAG&BIT0)
        {
            OUTPUT_TIME_SCREEN();
            TIME_FLAG &= ~BIT0;
        }
        //======��������===============================================================
        //======RF����==============================================================
        if(nRF_ON&BIT1)
        {
            if(nRF24L01_RxPacket(RxBuf))
            {
                OUTPUT_IMAGE_FILE("TEST0001TXT",1);
            }
            nRF_ON &= ~BIT1;
        }
        //==========================================================================


        mmc_GoIdle();              //����MMC/SD����Idleģʽ����ȡ�����������ϲ鿴����
        __low_power_mode_3();
        LPM3;
    }

}
