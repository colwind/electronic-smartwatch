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

uchar long_year(uint a)
{
    if ((time.year%4==0&&time.year%100!=0)||(time.year%400==0))
        return (1);
        else return (0);
}

const uchar mon[12]= {6,2,2,5,0,3,5,1,4,6,2,4};
const uchar mon_r[12]= {5,1,2,5,0,3,5,1,4,6,2,4};/****这是闰年****/
uchar year_code(uint a)
{
   uchar t;
   a=a % 100;
   t=a;
   a=t+(t/4);
   a=a % 7;
   return(a);
}
main()
{
    scanf("%d%d%d",&time.year,&time.month,&time.day);


    if(long_year(time.year))
    {
        time.weekday=(time.day+mon_r[time.month-1]+year_code(time.year))% 7;
    }
    else
    {
        time.weekday=(time.day+mon[time.month-1]+year_code(time.year))% 7;
    }
          /**********0为周日***************************************/
    printf ("%d",time.weekday);
    return 0;
}






