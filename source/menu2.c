
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
uchar set_flag;/***һ���˵�1������ 2��Ӧ�ã�3�����߿���4������*****�����˵�5�����ӿ��� 6�����Ӷ�ʱ��7�����8��ʱ������9�����㱨ʱ����***/

uchar key_flag;/**********����{ 1����  2����  3��ȷ��  4������}*******/



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
    printf("1menu");/*****���set_flag1����;*****/
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
                    output_scream(set_flag);/*******���set_flag����;************/
                    break;
                case 2:
                    set_flag++;
                    if (set_flag==5) set_flag=1;
                    output_scream(set_flag);/***���set_flag����;**/
                    break;
                case 3:
                    switch (set_flag)
                    {
                    case 1:
                        set_flag=5;
                        output_scream(set_flag);/***����������Ӳ˵�set_flag����**/
                        break;
                    case 2:
                        set_flag=7;
                        output_scream(set_flag);/***�������Ӧ�ò˵�set_flag����**/
                        break;
                    case 3:
                      //  if (nRF_on)
                       //     else   /************�ж����߿��ش���񣬲��ؿ�**********************/
                        printf("on or off");/***************������߿��ؽ���************************/
                        break;
                    case 4:
                        set_flag=8;
                        output_scream(set_flag);/****����������ò˵�set_flag����****/
                        break;
                    }
                    break;
                case 4:
                    printf("������");/**���������**/
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
            //        if (set_flag==6) set_flag=7; printf();/***��������˵�set_flag����****/ֻ�����
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
            //        if (set_flag==4) set_flag=3;  printf();/****��������˵�set_flag����*****/
                case 3:
                    switch (set_flag)
                    {
                    case 5:
                      //  if (���ӹر�) ��
                       //     else �ر�
                        printf("�л�����");/*****���ӵ�ǰ����********/
                        break;
                    case 6:
                        printf("���Ӷ�ʱ");
                        /**���Ӷ�ʱ����**/
                        break;
                    case 7:
                        printf("������");
                        /***������***/
                        break;
                    case 8:
                        printf("ʱ���趨");
                        /****ʱ���趨����*******/
                        break;
                    case 9:
                     //   if (����ر�) ��
                     //       else �ر�
                        printf("�л����㱨ʱ");
                        /**���㱨ʱ��ǰ����**/
                        break;
                    }
                    break;
                case 4:
                    set_flag=1;
                    printf("1menu");/**���set_flag1����**/
                    break;
            }
        }
        key_flag=0;
    }
}










