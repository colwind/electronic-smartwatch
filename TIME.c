

//���������մ�����������������ڼ����ο��ٶȾ���
const uchar mon[12]= {6,2,2,5,0,3,5,1,4,6,2,4};//�´���
const uchar mon_r[12]= {5,1,2,5,0,3,5,1,4,6,2,4};//������´���

/*******************************************************************************
*�������ƣ�long_year
*�������ܣ������Ƿ�Ϊ����
*˵����
*******************************************************************************/
uchar long_year(uint a)
{
    if ((time.year%4==0&&time.year%100!=0)||(time.year%400==0))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

/*******************************************************************************
*�������ƣ�year_code
*�������ܣ����������
*˵����
*******************************************************************************/
uchar year_code(uint a)
{
    uchar t;
    a=a % 100;
    t=a;
    a=t+(t/4);
    a=a % 7;
    return(a);
}
/*******************************************************************************
*�������ƣ�weekday
*�������ܣ�����ṹ����ʱ�����ڼ�
*˵����������ԴΪ�ṹ��,������ֱ�ӷ���ṹ��֮��
*******************************************************************************/
void weekday()
{
    
    if(TIME_FLAG&BIT1)
    {
        time.weekday=(time.day+mon_r[time.month-1]+year_code(time.year))% 7;
    }
    else
    {
        time.weekday=(time.day+mon[time.month-1]+year_code(time.year))% 7;
    }
    
}
/*******************************************************************************
*�������ƣ�Processing_Time
*�������ܣ�����ʱ�䣬���ڿ��Ź�������ʱ�����
*˵��������Ϊ��λ�����й����ֱ�ʱ���һϵ�д���
*******************************************************************************/
void Processing_Time()
{
    time.s++;
    if (time.s==60)
    {
        time.s=0;
        time.m++;
        TIME_FLAG |= BIT0;//�����¼�
        if (time.m==60)
        {
            time.m=0;
            time.h++;
            if (time.h==24)
            {
                time.h=0;
                time.day++;
                weekday();
                if (time.day>=29)
                {
                    if (time.month==2)
                    {
                        if (TIME_FLAG==0)  /*********ƽ��*****************/
                        {
                            time.day=1;
                            time.month++;
                        } 
                        else/*********����*****************/
                        {
                            if (time.day==30)
                            {
                                time.day=1;
                                time.month++;
                            }
                        }
                    }
                    else
                    {
                        
                        if(time.day>=31)
                        {
                            if ((time.month==4)||(time.month==6)||(time.month==9)||(time.month==11))/*********С��********/
                            {
                                time.day=1;
                                time.month++;
                            }
                            else
                            {
                                if(time.day==32)
                                {
                                    if (time.month==12)
                                    {
                                        time.year++;
                                        if (long_year(time.year))
                                        {
                                            TIME_FLAG |= BIT1;
                                        }
                                        else 
                                        {
                                            TIME_FLAG &= ~BIT1;
                                            time.month=1;
                                            time.day=1;
                                        }/*******************����Ϊ1.ƽ��Ϊ0*************************/
                                        
                                    }
                                    else
                                    {
                                        time.month++;
                                        time.day=1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*******************************************************************************
*�������ƣ�Time_init
*�������ܣ�ʱ���ʼ��
*˵������SD���ж���ʱ�䣬������
*******************************************************************************/
void Time_init()
{
    /*
    
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
    */
    
    
    time.year=2015;
    time.month=4;
    time.day=3;
    time.weekday=1;
    time.h=22;
    time.m=18;
    time.s=1;
    //�ж�����
    if (long_year(time.year))
    {
        TIME_FLAG |= BIT1;
    }
    else
    {
        TIME_FLAG &= ~BIT1;
    }
    weekday();
    TIME_FLAG |= BIT0;
    
}




