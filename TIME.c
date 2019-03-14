

//运用年月日代码计算任意日期星期几，参考百度经验
const uchar mon[12]= {6,2,2,5,0,3,5,1,4,6,2,4};//月代码
const uchar mon_r[12]= {5,1,2,5,0,3,5,1,4,6,2,4};//闰年的月代码

/*******************************************************************************
*函数名称：long_year
*函数功能：计算是否为闰年
*说明：
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
*函数名称：year_code
*函数功能：计算年代码
*说明：
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
*函数名称：weekday
*函数功能：计算结构体中时间星期几
*说明：数据来源为结构体,计算结果直接放入结构体之中
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
*函数名称：Processing_Time
*函数功能：处理时间，用于看门狗函数的时间计算
*说明：以秒为单位，进行关于手表时间的一系列处理
*******************************************************************************/
void Processing_Time()
{
    time.s++;
    if (time.s==60)
    {
        time.s=0;
        time.m++;
        TIME_FLAG |= BIT0;//分钟事件
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
                        if (TIME_FLAG==0)  /*********平年*****************/
                        {
                            time.day=1;
                            time.month++;
                        } 
                        else/*********闰年*****************/
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
                            if ((time.month==4)||(time.month==6)||(time.month==9)||(time.month==11))/*********小月********/
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
                                        }/*******************闰年为1.平年为0*************************/
                                        
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
*函数名称：Time_init
*函数功能：时间初始化
*说明：从SD卡中读出时间，不读秒
*******************************************************************************/
void Time_init()
{
    /*
    
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
    */
    
    
    time.year=2015;
    time.month=4;
    time.day=3;
    time.weekday=1;
    time.h=22;
    time.m=18;
    time.s=1;
    //判断闰年
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




