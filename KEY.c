#define Key_backBIT		BIT5//P1

#define Key_L1BIT		BIT6//P1

#define Key_R2BIT		BIT7//P1

#define Key_enter3BIT	BIT8//P1
/*全局变量*/
uchar Screen=0;
uchar Key_flag=4;//键盘按键标志，0-3表示键盘，4表示无动作
uchar Set_cur=0;
void Screen_refresh(uchar num);
/*******************************************************************************
*函数名称：init_key_port
*函数功能：初始化键盘
*说明：
*******************************************************************************/
void init_key_port()
{
  P1DIR &= ~(Key_backBIT+Key_L1BIT+Key_R2BIT);
  P2DIR &= ~(Key_enter3BIT);
  P1SEL &= ~(Key_backBIT+Key_L1BIT+Key_R2BIT);
  P2SEL &= ~(Key_enter3BIT);
  
  P1IES &= ~(Key_backBIT+Key_L1BIT+Key_R2BIT);
  P2IES &= ~(Key_enter3BIT);
  
  P1IE |= (Key_backBIT+Key_L1BIT+Key_R2BIT);
  P2IE |= (Key_enter3BIT);
  
}
/*******************************************************************************
*函数名称：Key_int_ban
*函数功能：开关键盘中断
*说明：ban为1禁止键盘中断
*******************************************************************************/
void Key_int_ban(uchar ban)
{
	if(ban)
	{
		P1IE &= ~(Key_backBIT+Key_L1BIT+Key_R2BIT);
		P2IE &= ~(Key_enter3BIT);
	}
	else
	{
		P1IE |= (Key_backBIT+Key_L1BIT+Key_R2BIT);
		P2IE |= (Key_enter3BIT);
	}
}
/*******************************************************************************
*函数名称：P_Key_Input
*函数功能：处理键盘输入
*说明：处理键盘输入，主要决定下一步动作，少有直接执行
*******************************************************************************/
void P_Key_Input()
{
  switch(Screen)
  {
  case 0://时间屏幕
    Screen_refresh(1);
    break;
  case 1://欢迎屏幕
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(0);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      Screen_refresh(2);
      break;
    }
    break;
  case 2://功能选单
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(1);
      break;
    case 1:
      break;
    case 2:
      Screen_refresh(3);
      break;
    case 3:
      if(TIME_FLAG&BIT3)
      {
        Screen_refresh(7);
      }
      else
      {
        Screen_refresh(8);
      }

      break;
    }
    break;
  case 3:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(1);
      break;
    case 1:
      Screen_refresh(2);
      break;
    case 2:
      if(nRF_ON&BIT0)
      {
        Screen_refresh(4);
      }
      else
      {
        Screen_refresh(5);
      }
      break;
    case 3:
      Screen_refresh(11);
      break;
    }
    break;
  case 4:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(1);
      break;
    case 1:
      Screen_refresh(3);
      break;
    case 2:
      Screen_refresh(6);
      break;
    case 3:
      RF_Power(0);
      Screen_refresh(5);
      break;
    }
    break;
  case 5:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(1);
      break;
    case 1:
      Screen_refresh(3);
      break;
    case 2:
      Screen_refresh(6);
      break;
    case 3:
      RF_Power(1);
      Screen_refresh(4);
      break;
    }
    break;
  case 6:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(1);
      break;
    case 1:
      if(nRF_ON&BIT0)
      {
        Screen_refresh(4);
      }
      else
      {
        Screen_refresh(5);
      }
      break;
    case 2:
      break;
    case 3:
      Screen_refresh(13);
      break;
    }
    break;
  case 7:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(2);
      break;
    case 1:
      break;
    case 2:
      Screen_refresh(9);
      break;
    case 3:
      TIME_FLAG &= ~BIT3;
      break;
    }
    break;
  case 8:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(2);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      TIME_FLAG |= BIT3;
      Screen_refresh(7);
      break;
    }
    break;
  case 9:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(2);
      break;
    case 1:
      if(TIME_FLAG&BIT3)
      {
        Screen_refresh(7);
      }
      else
      {
        Screen_refresh(8);
      }
      break;
    case 2:
      break;
    case 3:
      Screen_refresh(10);
      break;
    }
    break;
  case 10:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(2);
      break;
    case 1:
      switch(Set_cur)
      {
      case 1:
        if(alarm.h<23)
        {
          alarm.h++;
        }
        else
        {
          alarm.h=0;
        }
        break;
      case 2:
        if(alarm.m<60)
        {
          alarm.m++;
        }
        else
        {
          alarm.m=0;
        }
        break;
      }
      break;
    case 2:
      if(Set_cur==1)
      {
        P4OUT &= ~BIT5;
        
        //LED_ON(3);
       // LED_ON(4);
        //LED_OFF(1);
        //LED_OFF(2);
        Set_cur=2;
      }
      else
      {
        P4OUT |= BIT5;
       // LED_ON(1);
       // LED_ON(2);
       // LED_OFF(3);
       // LED_OFF(4);
        Set_cur=1;
      }
      
      break;
    case 3:
      Screen_refresh(10);
      break;
    }
    
    break;
  case 11:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(2);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      if(nRF_ON&BIT0)
      {
        Screen_refresh(12);
      }
      else
      {
        Screen_refresh(5);
      }
      
      break;
    }
    break;
  case 12:
    switch(Key_flag)
    {
    case 0:
      nRF_ON &= ~BIT2;
      Screen_refresh(2);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    }
    break;
  case 13:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(6);
      break;
    case 1:
      break;
    case 2:
       if(TIME_FLAG&BIT2)
       {
         Screen_refresh(16);
       }
       else
       {
         Screen_refresh(15);
       }
      break;
    case 3:
      Screen_refresh(14);
      break;
    }
    break;
  case 14:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(2);
      break;
    case 1:
      switch(Set_cur)
      {
      case 1:
        if(time.h<23)
        {
          time.h++;
        }
        else
        {
          time.h=0;
        }
        break;
      case 2:
        if(time.m<60)
        {
          time.m++;
        }
        else
        {
          time.m=0;
        }
        break;
      }
      break;
    case 2:
      if(Set_cur==1)
      {
        P4OUT &= ~BIT5;
       // LED_ON(3);
        //LED_ON(4);
        //LED_OFF(1);
       // LED_OFF(2);
        //Set_cur=2;
      }
      else
      {
       // LED_ON(1);
       // LED_ON(2);
       // LED_OFF(3);
       // LED_OFF(4);
        Set_cur=1;
      }
      
      break;
    case 3:
      Screen_refresh(10);
      break;
    }
    break;
  case 15:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(6);
      break;
    case 1:
      Screen_refresh(13);
      break;
    case 2:
      if(TIME_FLAG&BIT4)
      {
        Screen_refresh(18);
      }
      else
      {
        Screen_refresh(17);
      }
      break;
    case 3:
      TIME_FLAG |= BIT4;
      break;
    }
    break;
  case 16:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(6);
      break;
    case 1:
      Screen_refresh(13);
      break;
    case 2:
      if(TIME_FLAG&BIT4)
      {
        Screen_refresh(18);
      }
      else
      {
        Screen_refresh(17);
      }
      break;
    case 3:
      TIME_FLAG &= ~BIT4;
      break;
    }
    break;
  case 17:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(6);
      break;
    case 1:
      break;
    case 2:
       if(TIME_FLAG&BIT2)
       {
         Screen_refresh(16);
       }
       else
       {
         Screen_refresh(15);
       }
      break;
    case 3:
      TIME_FLAG |= BIT4;
      Screen_refresh(18);
      break;
    }
    break;
  case 18:
    switch(Key_flag)
    {
    case 0:
      Screen_refresh(6);
      break;
    case 1:
      break;
    case 2:
       if(TIME_FLAG&BIT2)
       {
         Screen_refresh(16);
       }
       else
       {
         Screen_refresh(15);
       }
      break;
    case 3:
      TIME_FLAG &= ~BIT4;
      Screen_refresh(17);
      break;
    }
    break;
  }
  Key_flag=4;
}
/*******************************************************************************
*函数名称：Screen_refresh
*函数功能：屏幕刷新
*说明：执行键盘操作过后产生的各种效果
*******************************************************************************/
void Screen_refresh(uchar num)
{
  Screen=num;
  switch(Screen)
  {
  case 0:
    ClocK_Mode=1;//打开钟表模式
    OUTPUT_TIME_SCREEN();
    
    break;
  case 1:
    break;
  case 2:
    OUTPUT_S_TIME("MENU0002TXT");
    break;
  case 3:
    OUTPUT_S_TIME("MENU0003TXT");
    break;
  case 4:
    OUTPUT_S_TIME("MENU0004TXT");
    break;
  case 5:
    OUTPUT_S_TIME("MENU0005TXT");
    break;
  case 6 :
    OUTPUT_S_TIME("MENU0006TXT");
    break;
  case 7:
    OUTPUT_S_TIME("MENU0007TXT");
    break;
  case 8:
    OUTPUT_S_TIME("MENU0008TXT");
    break;
  case 9:
    OUTPUT_S_TIME("MENU0009TXT");
    break;
  case 10://*************************************************************************************
    //LED_ON(1);
    P4OUT &= ~BIT5;
    Set_cur=1;
    break;
  case 11:
    OUTPUT_S_TIME("MENU0011TXT");
    break;
  case 12://***********************************************************************************
    nRF_ON |= BIT2;
    break;
  case 13:
    OUTPUT_S_TIME("MENU0013TXT");
    break;
  case 14://***************************************************************************************
    break;
  case 15:
    OUTPUT_S_TIME("MENU0015TXT");
    break;
  case 16:
    OUTPUT_S_TIME("MENU0016TXT");
    break;
  case 17:
    OUTPUT_S_TIME("MENU0017TXT");
    break;
  case 18:
    OUTPUT_S_TIME("MENU0018TXT");
    break;
  }
  Key_flag=4;
}