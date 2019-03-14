
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
uchar set_flag;/***一级菜单1：闹钟 2：应用：3：无线开关4：设置*****二级菜单5：闹钟开关 6：闹钟定时：7：码表8：时间设置9：整点报时开关***/

uchar key_flag;/**********按键{ 1：左  2：右  3：确认  4：返回}*******/



void output_scream(uchar a)
{
    switch (a)
    {
        case 1:
            printf("1menu");
            break;
        case 2:
            printf("2menu");
            break;
        case 3:
            printf("3menu");
            break;
        case 4:
            printf("4menu");
            break;
        case 5:
            printf("5menu");
            break;
        case 6:
            printf("6menu");
            break;
        case 7:
            printf("7menu");
            break;
        case 8:
            printf("8menu");
            break;
        case 9:
            printf("9menu");
            break;

    }
}
main()
{
    set_flag=1;
    printf("1menu");/*****输出set_flag1界面;*****/
    while (1)
    {
        scanf("%d",&key_flag);
        if (set_flag<5)
        {
            switch (key_flag)
            {
                case 1:
                    set_flag--;
                    if (set_flag==0) set_flag=4;
                    output_scream(set_flag);/*******输出set_flag界面;************/
                    break;
                case 2:
                    set_flag++;
                    if (set_flag==5) set_flag=1;
                    output_scream(set_flag);/***输出set_flag界面;**/
                    break;
                case 3:
                    switch (set_flag)
                    {
                    case 1:
                        set_flag=5;
                        output_scream(set_flag);/***输出二级闹钟菜单set_flag界面**/
                        break;
                    case 2:
                        set_flag=7;
                        output_scream(set_flag);/***输出二级应用菜单set_flag界面**/
                        break;
                    case 3:
                      //  if (nRF_on)
                       //     else   /************判断无线开关打开与否，并关开**********************/
                        printf("on or off");/***************输出无线开关界面************************/
                        break;
                    case 4:
                        set_flag=8;
                        output_scream(set_flag);/****输出二级设置菜单set_flag界面****/
                        break;
                    }
                    break;
                case 4:
                    printf("主界面");/**输出主界面**/
                    set_flag=1;
                    break;
                   // exit;
            }
        }
        else
        {
            switch (key_flag)
            {
                case 1:

                    set_flag--;
                    if (set_flag==4)
                    {
                        set_flag+=2;
                        output_scream(set_flag);
                    }
                    if (set_flag==7)
                    {
                        set_flag+=2;
                        output_scream(set_flag);
                    }
                    break;
            //        if (set_flag==6) set_flag=7; printf();/***输出二级菜单set_flag界面****/只有码表
                case 2:
                    set_flag++;
                    if (set_flag==7)
                    {
                        set_flag-=2;
                        output_scream(set_flag);
                    }
                    if (set_flag==10)
                    {
                        set_flag-=2;
                        output_scream(set_flag);
                    }
                    break;
            //        if (set_flag==4) set_flag=3;  printf();/****输出二级菜单set_flag界面*****/
                case 3:
                    switch (set_flag)
                    {
                    case 5:
                      //  if (闹钟关闭) 打开
                       //     else 关闭
                        printf("切换闹钟");/*****闹钟当前界面********/
                        break;
                    case 6:
                        printf("闹钟定时");
                        /**闹钟定时界面**/
                        break;
                    case 7:
                        printf("码表界面");
                        /***码表界面***/
                        break;
                    case 8:
                        printf("时间设定");
                        /****时间设定界面*******/
                        break;
                    case 9:
                     //   if (整点关闭) 打开
                     //       else 关闭
                        printf("切换整点报时");
                        /**整点报时当前界面**/
                        break;
                    }
                    break;
                case 4:
                    set_flag=1;
                    printf("1menu");/**输出set_flag1界面**/
                    break;
            }
        }
        key_flag=0;
    }
}










