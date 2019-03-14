/*******************************************************************************
*此段程序定义一部分公用宏定义或函数
*******************************************************************************/
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_100nS(x) __delay_cycles((long)(CPU_F*(double)x/10000000.0)) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 
#define uchar unsigned char
#define ulong unsigned long
#define uint unsigned int
/********************************函数声明区************************************/
uchar long_year(uint a);
uchar year_code(uint a);
void weekday();
void Processing_Time();
void Time_init();
/*******************************************************************************
*函数名称：WDT_Init
*函数功能：看门狗初始化
*说明：用32768Hz晶体作为ACLK，看门狗定时1s作为时钟定时器
*******************************************************************************/
void WDT_Init()
{
    //WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
    WDTCTL = WDT_ADLY_1000;                     // Set Watchdog Timer interval to 1000ms
    IE1 |= WDTIE;                             // Enable WDT interrupt
}
/*******************************************************************************
*函数名称：Clock_Init_Ex32768
*函数功能：使用32768晶体的系统时钟初始化函数
*说明：初期测试不使用
*******************************************************************************/
void Clock_Init_Ex32768()
{
  uchar i;

  BCSCTL2|=SELM1 + SELM0 + SELS;    //MCLK为32.768KHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG; 
}
/*******************************************************************************
*函数名称：Clock_Init
*函数功能：使用8M晶体的系统时钟初始化函数
*说明：初期测试使用
*******************************************************************************/
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG; 
}
