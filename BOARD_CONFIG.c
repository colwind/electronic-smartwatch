/*******************************************************************************
*�˶γ�����һ���ֹ��ú궨�����
*******************************************************************************/
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_100nS(x) __delay_cycles((long)(CPU_F*(double)x/10000000.0)) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 
#define uchar unsigned char
#define ulong unsigned long
#define uint unsigned int
/********************************����������************************************/
uchar long_year(uint a);
uchar year_code(uint a);
void weekday();
void Processing_Time();
void Time_init();
/*******************************************************************************
*�������ƣ�WDT_Init
*�������ܣ����Ź���ʼ��
*˵������32768Hz������ΪACLK�����Ź���ʱ1s��Ϊʱ�Ӷ�ʱ��
*******************************************************************************/
void WDT_Init()
{
    //WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
    WDTCTL = WDT_ADLY_1000;                     // Set Watchdog Timer interval to 1000ms
    IE1 |= WDTIE;                             // Enable WDT interrupt
}
/*******************************************************************************
*�������ƣ�Clock_Init_Ex32768
*�������ܣ�ʹ��32768�����ϵͳʱ�ӳ�ʼ������
*˵�������ڲ��Բ�ʹ��
*******************************************************************************/
void Clock_Init_Ex32768()
{
  uchar i;

  BCSCTL2|=SELM1 + SELM0 + SELS;    //MCLKΪ32.768KHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG; 
}
/*******************************************************************************
*�������ƣ�Clock_Init
*�������ܣ�ʹ��8M�����ϵͳʱ�ӳ�ʼ������
*˵�������ڲ���ʹ��
*******************************************************************************/
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //��XT2����
  BCSCTL2|=SELM1+SELS;              //MCLKΪ8MHZ��SMCLKΪ8MHZ
  do{
    IFG1&=~OFIFG;                   //������������־
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //�����־λ1�������ѭ���ȴ�
  IFG1&=~OFIFG; 
}
