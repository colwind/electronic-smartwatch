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
uchar set_flag1;/***һ���˵�1������ 2��Ӧ�ã�3�����߿���4������***/
uchar set_flag2;/***�����˵�1�����ӿ��� 2�����Ӷ�ʱ��3�����4��ʱ������5�����㱨ʱ����***/


uchar key_flag;/**********����{ 1����  2����  3��ȷ��  4������}*******/


main()
{
    set_flag2=0;
    set_flag1=1;
    printf()/*****���set_flag1����;*****/
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
                    printf();/*******���set_flag1����;************/
                case 2:
                    set_flag1++;
                    if (set_flag1==5) set_flag1=1;
                    printf();/***���set_flag1����;**/
                case 3:
                    switch (set_flag1)
                    {
                    case 1:
                        set_flag2=1;
                        printf();/***����������Ӳ˵�set_flag2����**/
                    case 2:
                        set_flag2=3;
                        printf();/***�������Ӧ�ò˵�set_flag2����**/
                    case 3:
                        if (nRF_on)
                            else   /************�ж����߿��ش���񣬲��ؿ�**********************/
                        printf()/***************������߿��ؽ���************************/
                    case 4:
                        set_flag2=4;
                        printf()/****����������ò˵�set_flag2����****/
                    }
                case 4:
                    printf();/**���������**/
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
                    printf();/***��������˵�set_flag2����****/
                case 2:
                    set_flag2++��
                    if(set_flag2==3||set_flag2==6) set_flag2-=2;
                    if (set_flag2==4) set_flag2=3;
                    printf();/****��������˵�set_flag2����*****/
                case 3:
                    switch (set_flag2)
                    {
                    case 1:
                        if (���ӹر�) ��
                            else �ر�
                        printf();/*****���ӵ�ǰ����********/
                    case 2:
                        /**���Ӷ�ʱ����**/
                    case 3:
                        /***������***/
                    case 4:
                        /****ʱ���趨����*******/
                    case 5:
                        if (����ر�) ��
                            else �ر�
                        /**���㱨ʱ��ǰ����**/
                    }
                case 4:
                    set_flag2=0��
                    set_flag1=1��
                    printf();/**���set_flag1����**/
            }
        }
    }
}











main()
{
    set_flag1=1;
    printf("");   //���������
    while(1)
    {
        scanf("%d",&key_flag);
        switch (key_flag)
        {
            case 1:
                set_flag1--;
                if (set_flag1==0) set_flag1=4;
                printf()//���set_flag1����;
            case 2:
                set_flag1++;
                if (set_flag1==5) set_flag1=1;
                printf()//���set_flag1����;
            case 3:
                printf()//��������˵�
        }
    }


}



