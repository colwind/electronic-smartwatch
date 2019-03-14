/******************************************************************************
*此段程序实现对FAT16文件系统下的SD卡文件读写
*该系统并未包含FAT所有功能，所使用FAT系统相对简单，且仅支持FAT和SD卡
*MSP430存储数据结构为小端，即低字节在低位地址，高字节在高位地址
*SD卡先在电脑上格式化为FAT，簇大小32K以下
*SD卡插入卡座时必须断电或者用仿真器暂停程序，否则可能无法操作
*******************************************************************************/
#include "SPI.c"
#include "MMC.c"
#include "FAT.c"
/********************************定义全局变量**********************************/
ulong cardSize = 0;
uchar status = 1;
uint timeout = 0;
extern uint8 FAT32_EN;                   //文件系统标志变量，为1时表示FAT32系统，0表示FAT系统
extern uint16 DirectoryCount,FileCount;  //根目录下文件夹和文件的数量
/******************************************************************************/
/*******************************************************************************
*函数名称：NoCardRing
*函数功能：SD卡无卡报警
*说明：暂时保留
*******************************************************************************/
void NoCardRing()
{
}

/*******************************************************************************
*函数名称：INIT_SD_Card
*函数功能：SD卡的SPI初始化以及SD卡的初始化
*说明：SD卡使用硬件SPI进行通信
*******************************************************************************/
void INIT_SD_Card ()
{
    while (status != 0)                                        
    {
        status = initMMC();     //如果返回非零值，则表示出错，SD卡继续重新初始化 
        timeout++;
        if (timeout == 50) //初始化SD卡，如果依然返回错误，则初始化失败，可能是没有SD卡或损坏
        {
            NoCardRing();//报警，提示MMC/SD卡初始化失败，请检查是否安装好，或者格式化好卡
            break;       //50次满则退出
        }
    }
     //while ((mmc_ping() != MMC_SUCCESS));    //等待MMC/SD卡插入，这里不具备监测功能
    cardSize =  MMC_ReadCardSize();           //从CSD寄存器读取MMC/SD卡容量大小，返回长整型
    ReadBPB();                                //读MMC/SD卡的BPB区信息
     //FormatCard();                             //格式化MMC/SD卡，需要时使用
    FATSystemType();                          //MMC/SD卡文件系统类型判断，FAT32_EN==1则为FAT32文件系统
    // if(FAT32_EN==1) ReadBPB32();
    FileCountSearch();                        //搜索文件夹及文件数量
    //DelFile("TEST0001TXT");                   //删除指定文件函数，需要时可以使用
    //FileCountSearch();                      //搜索文件夹及文件数量
    if(FAT32_EN)
    {
        NoCardRing();//文件系统为FAT32，本程序暂时不支持FAT32，如果继续将会发生错误
    }
}
/*******************************************************************************
*函数名称：OperateFile_EX
*函数功能：可以修正偏移的文件读写函数
*说明：只能对512字节大小的buffer进行操作，只能进行读操作
*******************************************************************************/
void OperateFile_EX(uint8 Name[11], uint32 Start, uchar * Data)
{
    if(Start%512)
    {//存在偏移，需要用两个簇进行组合
        uchar temp[512];
        uint pure,i,j;
        pure=Start;
        while(pure>512)
        {
            pure=pure-512;
        }
        OperateFile(0,Name,(Start-pure),512,temp);
        for(i=0,j=pure;j<512;i++,j++)
        {
            Data[i]=temp[j];
        }
        OperateFile(0,Name,(Start-pure+512),512,temp);
        for(i=0;i<pure;i++)
        {
            Data[i+512-pure]=temp[i];
        }
        
    }
    else
    {
        OperateFile(0,Name,Start,512,Data);
    }
}
