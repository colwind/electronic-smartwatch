#include<stdio.h>
#define uchar unsigned char
#define uint unsigned int
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
uchar set_flag1;/***一级菜单1：闹钟 2：应用：3：无线开关4：设置***/
uchar set_flag2;/***二级菜单1：闹钟开关 2：闹钟定时：3：码表4：时间设置5：整点报时开关***/


uchar key_flag;/**********按键{ 1：左  2：右  3：确认  4：返回}*******/


main()
{
    set_flag2=0;
    set_flag1=1;
    printf()/*****输出set_flag1界面;*****/
    while (1)
    {
        scanf("%d",&key_flag);
        if (set_flag2==0)
        {
            switch (key_flag)
            {
                case 1:
                    set_flag1--;
                    if (set_flag1==0) set_flag1=4;
                    printf();/*******输出set_flag1界面;************/
                case 2:
                    set_flag1++;
                    if (set_flag1==5) set_flag1=1;
                    printf();/***输出set_flag1界面;**/
                case 3:
                    switch (set_flag1)
                    {
                    case 1:
                        set_flag2=1;
                        printf();/***输出二级闹钟菜单set_flag2界面**/
                    case 2:
                        set_flag2=3;
                        printf();/***输出二级应用菜单set_flag2界面**/
                    case 3:
                        if (nRF_on)
                            else   /************判断无线开关打开与否，并关开**********************/
                        printf()/***************输出无线开关界面************************/
                    case 4:
                        set_flag2=4;
                        printf()/****输出二级设置菜单set_flag2界面****/
                    }
                case 4:
                    printf();/**输出主界面**/
                    exit;
            }
        }
        else
        {
            switch (key_flag)
            {
                case 1:
                    set_flag2--;
                    if (set_flag2==0||set_flag2==3) set_flag2+=2
                    if (set_flag2==2) set_flag2=3;
                    printf();/***输出二级菜单set_flag2界面****/
                case 2:
                    set_flag2++；
                    if(set_flag2==3||set_flag2==6) set_flag2-=2;
                    if (set_flag2==4) set_flag2=3;
                    printf();/****输出二级菜单set_flag2界面*****/
                case 3:
                    switch (set_flag2)
                    {
                    case 1:
                        if (闹钟关闭) 打开
                            else 关闭
                        printf();/*****闹钟当前界面********/
                    case 2:
                        /**闹钟定时界面**/
                    case 3:
                        /***码表界面***/
                    case 4:
                        /****时间设定界面*******/
                    case 5:
                        if (整点关闭) 打开
                            else 关闭
                        /**整点报时当前界面**/
                    }
                case 4:
                    set_flag2=0；
                    set_flag1=1；
                    printf();/**输出set_flag1界面**/
            }
        }
    }
}











main()
{
    set_flag1=1;
    printf("");   //输出主界面
    while(1)
    {
        scanf("%d",&key_flag);
        switch (key_flag)
        {
            case 1:
                set_flag1--;
                if (set_flag1==0) set_flag1=4;
                printf()//输出set_flag1界面;
            case 2:
                set_flag1++;
                if (set_flag1==5) set_flag1=1;
                printf()//输出set_flag1界面;
            case 3:
                printf()//输出二级菜单
        }
    }


}



