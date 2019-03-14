
//*************************************************************************
//               MSP430串口初始化
//*************************************************************************
void UART_Init()
{
  U0CTL=SWRST + CHAR;                //复位SWRST，8位数据模式
  
  U0TCTL|=SSEL1;                      //SMCLK为串口时钟
  U0BR1=baud_h;                       //BRCLK=8MHZ,Baud=BRCLK/N
  U0BR0=baud_l;                       //N=UBR+(UxMCTL)/8
  U0MCTL=0x00;                        //微调寄存器为0，波特率9600bps
  ME1|=UTXE0;                         //UART0发送使能
  //ME1|=URXE0;                         //UART0接收使能
  U0CTL&=~SWRST;
  IE1|=URXIE0;                        //接收中断使能位
  
  P3SEL|= BIT4 + BIT5;                //设置IO口为第二功能模式，启用UART功能
  P3DIR|= BIT4;                       //设置TXD0口方向为输出
}

//*************************************************************************
//              串口0发送数据函数
//*************************************************************************

void Send_Byte(uchar data)
{

  while(!(IFG1 & UTXIFG0));         //发送寄存器空的时候发送数据
    U0TXBUF=data;
}


void printstr(uchar *s)
{
    while(*s != '\0')
    {
        Send_Byte(*s++);
    }
}