#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
uint Month_Flag_1 =0xD5;
uint Month_Flag_2 =0x0A;

typedef struct TIME
{
 uint year;
 uchar month;
 uchar day;
 uchar weekday;
 uchar h;
 uchar m;
 uchar s;
} TIME;

TIME time;
uchar year_flag; //闰年记录
uchar long_year(uint a)
{
    if ((time.year%4==0&&time.year%100!=0)||(time.year%400==0))
        return (1);
        else return (0);
}

void delay(long int a)
{
    long int i;
    for (i=0;i<=a;i++);
    for (i=0;i<=a;i++);
}
main()
{/****************输入时间**************************************/
    scanf("%d%d%d%d%d%d",&time.year,&time.month,&time.day,&time.h,&time.m,&time.s);
    /*******************判断闰年****************************/
    if (long_year(time.year))
        year_flag=1;
    else year_flag=0;/*******************闰年为1.平年为0*************************/
    while (1)
    {
        time.s++;
        if (time.s==60)
        {
            time.s=0;
            delay(10000);
            time.m++;
            if (time.m==60)
            {
                time.m=0;
                time.h++;
                if (time.h==24)
                {
                    time.h=0;
                    time.day++;
                    if (time.day>=29)
                    {
                        if (time.month==2)
                        {
                            if (year_flag==0)  /*********平年*****************/
                            {
                                time.day=1;
                                time.month++;
                            } else/*********闰年*****************/
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
                                                year_flag=1;
                                            else year_flag=0;/*******************闰年为1.平年为0*************************/
                                            time.month=1;
                                            time.day=1;
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
                }system("cls");printf("%d.%d.%d   %d:%d:%d\n",time.year,time.month,time.day,time.h,time.m,time.s);
            }
        }

    }
}
