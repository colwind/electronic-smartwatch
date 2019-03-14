


/****nRF24L01���Ŷ���********************************************************************/

#define  RF24L01_CE_0        P4OUT &=~BIT2         //CE��P42   
#define  RF24L01_CE_1        P4OUT |= BIT2        

#define  RF24L01_CSN_0       P4OUT &=~BIT1         //CS��P41
#define  RF24L01_CSN_1       P4OUT |= BIT1     

#define  RF24L01_SCK_0       P4OUT &=~BIT0         //SCK��P40
#define  RF24L01_SCK_1       P4OUT |= BIT0   

#define  RF24L01_MISO        P3IN&BIT6             //MISO��P36
#define  RF24L01_MISO_P      BIT6

#define  RF24L01_MOSI_0      P3OUT &=~BIT7         //MOSI��P37
#define  RF24L01_MOSI_1      P3OUT |= BIT7

#define  IRQDIR              P1DIR &= ~BIT3        //IRQ P13 �ж����� �½���
#define  IRQSEL              P1SEL &= ~BIT3
#define  IRQIES              P1IES |= BIT3
#define  IRQIE               P1IE |= BIT3
  
/*****************************nRF24L01���߲�������****************************/
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  32  	// 32 TX payload
#define RX_PLOAD_WIDTH  32  	// 32 uints TX payload
/******************************NRF24L01�Ĵ���ָ��********************************/
#define READ_REG        0x00  	// ���Ĵ���ָ��
#define WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP1            0xFF  	// ����
/**************************SPI(nRF24L01)�Ĵ�����ַ****************************/
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
/*****************************RF24l01״̬����**********************************/
char  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���ص�ַ
char  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���յ�ַ
char  sta;
char  RxBuf[32]=
      { 0x01,0x02,0x03,0x4,0x05,0x06,0x07,0x08,
        0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
        0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,
        0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32};
/******************************************************************************/
void LED_IO_set(void);
void RF24L01_IO_set(void);
void LCD_IO_set();


void Delay(int s);
char SPI_RW(char data);
char SPI_Read(char reg);
char SPI_RW_Reg(char reg, char value);
char SPI_Read_Buf(char reg, char *pBuf, char uchars);
char SPI_Write_Buf(char reg, char *pBuf, char uchars);
void SetRX_Mode(void);
char nRF24L01_RxPacket(char* rx_buf);
void nRF24L01_TxPacket(char * tx_buf);
void init_NRF24L01(void);


 


//===========================RF24L01�˿�����==========================================
void RF24L01_IO_set(void)
{
      IRQDIR;
      IRQSEL;
      IRQIES;//IRQ���жϣ��½���
      IRQIE;
      
      P1IFG = 0x00;//IRQ��p14,���P1���ж�
      
      P3DIR &= ~BIT6;
      P3DIR |= BIT7;
      P3SEL &= ~(BIT6+BIT7);
      
      P4DIR |= (BIT0+BIT1+BIT2);
      P4SEL &= ~(BIT0+BIT1+BIT2);
    
    
    
    /*P1DIR &= ~BIT4;
      P1DIR |= BIT5;
      P1SEL &= ~(BIT4+BIT5);
      //P1IE &= ~(BIT4+BIT5);
      
      P2DIR |= BIT7;
      P2SEL &= BIT7;
      
      P3DIR &= ~BIT2;
      P3DIR |= (BIT1+BIT3);
      P3SEL &=~(BIT1+BIT2+BIT3);*/
}


//========================================����ʱ================================
void Delay(int s)
{
	unsigned int i,j;
	for(i=0; i<s; i++);
	for(j=0; j<s; j++);
}
//==============================================================================
//������uint SPI_RW(uint uchar)
//���ܣ�NRF24L01��SPIдʱ��
//******************************************************************************
char SPI_RW(char data)
{	
        char i,temp=0;
   	for(i=0;i<8;i++) // output 8-bit
   	{
	if((data & 0x80)==0x80)
	{
		RF24L01_MOSI_1;         // output 'uchar', MSB to MOSI
	}
	else
	{
	 	RF24L01_MOSI_0; 
	}
//==============================================================================
		data = (data << 1);            // shift next bit into MSB..
		temp<<=1;
		RF24L01_SCK_1;                // Set SCK high..
		if((RF24L01_MISO)==RF24L01_MISO_P)temp++;         // capture current MISO bit
		RF24L01_SCK_0;              // ..then set SCK low again
   	}
    return(temp);           		  // return read uchar  		
}
//****************************************************************************************************
//������uchar SPI_Read(uchar reg)
//���ܣ�NRF24L01��SPIʱ��
//****************************************************************************************************
char SPI_Read(char reg)
{
	char reg_val;
	RF24L01_CSN_0;           // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	RF24L01_CSN_1;         // CSN high, terminate SPI communication
	return(reg_val);       // return register value
}
//****************************************************************************************************/
//���ܣ�NRF24L01��д�Ĵ�������
//****************************************************************************************************/
char SPI_RW_Reg(char reg, char value)
{
	char status1;
	RF24L01_CSN_0;                   // CSN low, init SPI transaction
	status1 = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	RF24L01_CSN_1;                   // CSN high again
	return(status1);            // return nRF24L01 status uchar
}
//****************************************************************************************************/
//������uint SPI_Read_Buf(uchar reg, uchar *pBuf, uchar uchars)
//����: ���ڶ����ݣ�reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uchars���������ݵĸ���
//****************************************************************************************************/
char SPI_Read_Buf(char reg, char *pBuf, char chars)
{
	char status2,uchar_ctr;
	RF24L01_CSN_0;                    		// Set CSN low, init SPI tranaction
	status2 = SPI_RW(reg);       		// Select register to write to and read status uchar
	for(uchar_ctr=0;uchar_ctr<chars;uchar_ctr++)
        {
	pBuf[uchar_ctr] = SPI_RW(0);    // 
        }
	RF24L01_CSN_1;                           
	return(status2);   
}
//*********************************************************************************************************
//������uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
//����: ����д���ݣ�Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uchars��д�����ݵĸ���
//*********************************************************************************************************/
char SPI_Write_Buf(char reg, char *pBuf, char chars)
{
	char status1,uchar_ctr;
	RF24L01_CSN_0;             //SPIʹ��       
	status1 = SPI_RW(reg);   
	for(uchar_ctr=0; uchar_ctr<chars; uchar_ctr++) //
        {
	SPI_RW(*pBuf++);
        }
	RF24L01_CSN_1;           //�ر�SPI
	return(status1);    		  // 
}
//****************************************************************************************************/
//������void SetRX_Mode(void)
//���ܣ����ݽ������� 
//****************************************************************************************************/
void SetRX_Mode(void)
{
	RF24L01_CE_0 ;
        Delay(1);
	RF24L01_CE_1; 
	Delay(1000);//ע�ⲻ��̫С
}
//******************************************************************************************************/
//������unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//���ܣ����ݶ�ȡ�����rx_buf���ջ�������
//******************************************************************************************************/
char nRF24L01_RxPacket(char* rx_buf)
{
    char revale=0;
	sta=SPI_Read(STATUS);	     // ��ȡ״̬�Ĵ������ж����ݽ���״��		
	if(sta&0x40)                 // �ж��Ƿ���յ�����
	{
	    RF24L01_CE_0 ; 			//SPIʹ��
	    SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
	    revale =1;			//��ȡ������ɱ�־
	}
        else 
        {
              revale =0;	
        }
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	return revale;
}
//***********************************************************************************************************
//������void nRF24L01_TxPacket(char * tx_buf)
//���ܣ����� tx_buf������
//**********************************************************************************************************/
void nRF24L01_TxPacket(char * tx_buf)
{
	RF24L01_CE_0 ;			//StandBy Iģʽ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // װ������	
//	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	RF24L01_CE_1;		 //�ø�CE���������ݷ���
	Delay(600);
}
//****************************************************************************************
//NRF24L01��ʼ��
//***************************************************************************************/
void init_NRF24L01(void)
{

 	RF24L01_CE_0 ;    // chip enable
 	RF24L01_CSN_1;   // Spi disable 
 	RF24L01_SCK_0;   // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ-12dBm,����PA 0x02
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);   		// IRQ�շ�����ж���Ӧ��16λCRC	��������}
        Delay(1000);
}
/*******************************************************************************
*�������ƣ�RF_Power
*�������ܣ�RFģ�鿪��
*˵����0�ر�RF��1����RF
*******************************************************************************/
void RF_Power(uchar op)
{
    if(op)
    {
        nRF_ON |= BIT0;
        init_NRF24L01();
    }
    else
    {
        RF24L01_CE_0;
        SPI_RW_Reg(WRITE_REG + CONFIG, 0x0d);
        RF24L01_CE_1;
        delay_us(20);
        nRF_ON &= ~BIT0;
    }
}