//******************************************************************************
//FAT文件系统，支持文件读写、根目录文件与文件夹建立、子目录文件与文件夹建立、文件删除、文件系统格式化、文件搜索
//支持FAT16与FAT32文件系统，两类文件系统有着不一样的协议
//编译环境：IAR for MSP430 V5.10
//****************************************************************************** 
#include "mmc.h"
#include <stdio.h>
#include "FAT.h"
uchar BUFFER[512];

uint16 BPB_BytesPerSec;
static uint8 BPB_SecPerClus;
uint16 BPB_RsvdSecCnt;
uint8 BPB_NumFATs;
uint16 BPB_RootEntCnt;
uint16 BPB_TotSec16;
uint16 BPB_FATSz16;
uint32 BPB_HiddSec;

//uint32 BPB_FATSz32;
uint32 BPB_RootClus;
uint16 BPB_FSInfo;
uint8  FAT32_EN;                               //为0则表示FAT16，为1则表示FAT32

uint16 DirectoryCount = 0,FileCount = 0;       //根目录下文件夹和文件的数量

/***********************************************************************
函数功能：读一个扇区函数
操作内容：1、调用MMC/SD卡块读取函数
          2、函数宏定义参考头文件
***********************************************************************/

void ReadBlock(uint32 LBA)
{
	mmcReadSector(LBA,BUFFER);
        return;
}

/***********************************************************************
函数功能：写一个扇区函数
操作内容：1、调用MMC/SD卡块写入函数
          2、函数宏定义参考头文件
***********************************************************************/

void WriteBlock(uint32 LBA)
{
	mmcWriteSector(LBA,BUFFER);
        return;
}

/***********************************************************************
函数功能：字节数据复制
操作内容：数据复制，按字节方式，size定义大小
***********************************************************************/

void CopyBytes(void* S, void* D, uint16 size)
{
	uint8 *s = S, *d = D;
	uint16 i;
	for(i = 0; i < size; i++)
		*d++ = *s++;
}

/***********************************************************************
函数功能：字数据复制
操作内容：数据复制，按字方式，size定义大小
***********************************************************************/

void CopyWord(void* S, void* D, uint16 size)
{
	uint16 *s = S, *d = D;
	uint16 i;
	for(i = 0; i < size; i++)
		*d++ = *s++;
}

/***********************************************************************
函数功能：数据比较函数
操作内容：1、比较数据是否相等，相等返回1，不相等返回0
          2、参数为起始地址和数据大小
***********************************************************************/

uint8 IsEqual(void* A, void* B, uint8 Size)
{
	uint8 i, *a = A, *b = B;
	for(i = 0; i < Size; i++)
		if(a[i] != b[i])
			return 0;              //不相等返回0
	return 1;                              //相等返回1
}

/***********************************************************************
函数功能：空数据字节填充
操作内容：数据全为0，size定义填充0的大小
***********************************************************************/

void EmptyBytes(void* D, uint16 size)
{
	uint16 i;
	uint8* data = (uint8*)D;
	for(i = 0; i < size; i++)
	{
		*data++ = 0;
	}
}
/***********************************************************************
函数功能：写入MMC/SD卡FAT文件系统BPB(BIOS Parameter Block)数据结构
操作内容：1、BPB数据结构体参考头文件定义
          2、BPB数据包含磁盘的逻辑结构组成，如扇区大小、簇大小等信息
***********************************************************************/
void FATInit(void)
{
	FAT_BPB* BPB = (FAT_BPB*)BUFFER;
		
	BPB->BPB_BytesPerSec = 0x0200;          //每个扇区512字节
	BPB->BPB_SecPerClus  = 0x20;            //每个簇8个扇区
	BPB->BPB_RsvdSecCnt  = 0x06;          //一个保留扇区
	BPB->BPB_NumFATs     = 0x02;            //2个FAT表
	BPB->BPB_RootEntCnt  = 0x200;          //512目录项个数
	BPB->BPB_TotSec16    = 0x00;          //总扇区数
	BPB->BPB_FATSz16     = 0xF1;            //一个FAT表所占的扇区数
	BPB->BPB_HiddSec     = 0xFD;      //本FAT表前隐藏的扇区数
	
	WriteBlock(253);                          //写引导区
}


/***********************************************************************
函数功能：读取FAT16文件系统BPB(BIOS Parameter Block)数据结构
操作内容：1、BPB数据结构体参考头文件定义
          2、BPB数据包含磁盘的逻辑结构组成，如扇区大小、簇大小等信息
***********************************************************************/

void ReadBPB(void)
{
	FAT_BPB* BPB = (FAT_BPB*)BUFFER;
	ReadBlock(0);
        
        ReadBlock(BUFFER[0x1C6]);                  //获取参数，将BPB区数据读入缓冲区
        
	BPB_BytesPerSec = BPB->BPB_BytesPerSec;   //这些记录了数据结构  
	BPB_SecPerClus = BPB->BPB_SecPerClus;     //每簇扇区数，格式化时可以选每个簇的大小，如2K，32K
	BPB_RsvdSecCnt = BPB->BPB_RsvdSecCnt;     //MMC/SD卡格式化后，保留为6个扇区
	BPB_NumFATs = BPB->BPB_NumFATs;           //2个FAT表，如果一个FAT表损坏，可以用另外一个
	BPB_RootEntCnt = BPB->BPB_RootEntCnt;
	BPB_TotSec16 = BPB->BPB_TotSec16;         //如果为0，BPB_FATSz16记录扇区总数
	BPB_FATSz16 = BPB->BPB_FATSz16;           //FAT占用扇区总数，FAT32为0，FAT16不为0
	BPB_HiddSec = BPB->BPB_HiddSec;           //隐藏扇区，默认为0

}

/***********************************************************************
函数功能：读取FAT32文件系统BPB(BIOS Parameter Block)数据结构
操作内容：1、BPB数据结构体参考头文件定义
          2、BPB数据包含磁盘的逻辑结构组成，如扇区大小、簇大小等信息
***********************************************************************/

/*void ReadBPB32(void)
{
	FAT32_BPB* BPB = (FAT32_BPB*)BUFFER;
	ReadBlock(253);                             //获取参数，将BPB区数据读入缓冲区
        
	BPB_BytesPerSec = BPB->BPB_BytesPerSec;   //这些记录了数据结构  
	BPB_SecPerClus = BPB->BPB_SecPerClus;     //每簇扇区数，格式化时可以选每个簇的大小，如2K，32K
	BPB_RsvdSecCnt = BPB->BPB_RsvdSecCnt;     //MMC/SD卡格式化后，保留为6个扇区
	BPB_NumFATs = BPB->BPB_NumFATs;           //2个FAT表，如果一个FAT表损坏，可以用另外一个
	BPB_RootEntCnt = BPB->BPB_RootEntCnt;
	BPB_TotSec16 = BPB->BPB_TotSec16;         //如果为0，BPB_FATSz16记录扇区总数
	BPB_FATSz16 = BPB->BPB_FATSz16;           //FAT占用扇区总数，FAT32为0，FAT16不为0
	BPB_HiddSec = BPB->BPB_HiddSec;           //隐藏扇区，默认为0
        
        BPB_FATSz32 = BPB->BPB_FATSz32;
        BPB_RootClus =BPB->BPB_RootClus;
        BPB_FSInfo = BPB->BPB_FSInfo;
}

*/

/***********************************************************************
函数功能：获取FAT表扇区号，支持FAT16/32
操作内容：1、FAT表扇区号为隐藏扇区+保留扇区
          2、隐藏扇区默认为0
***********************************************************************/

uint32 FATStartSec(void)
{
    return BPB_RsvdSecCnt + BPB_HiddSec; //????????????????????????????
}

/***********************************************************************
函数功能：获取根目录开始扇区号，支持FAT32
操作内容：1、根目录扇区数是固定的
          2、根目录扇区紧接着FAT表之后
***********************************************************************/

uint32 DirStartSec(void)
{
  uint32 FATSz;
  if(FAT32_EN==0)
    FATSz=BPB_FATSz16;
  else
     ;
  //  FATSz=BPB_FATSz32;
    return BPB_HiddSec+BPB_RsvdSecCnt + BPB_NumFATs * FATSz;             //保留扇区+FAT表数×FAT占用扇区数   ??????????????????????
}

/***********************************************************************
函数功能：获取数据区开始扇区号，支持FAT32
操作内容：1、计算数据开始扇区号
          2、FAT32系统下BPB_RootEntCnt=0
***********************************************************************/

uint32 DataStartSec(void)
{
	return (uint32)(DirStartSec() + (BPB_RootEntCnt * 32) / BPB_BytesPerSec); //乘以32是因为根目录是32位长度文件
}

/***********************************************************************
函数功能：根目录占用扇区数，FAT16有效
操作内容：1、计算根目录占用扇区数
          2、根目录为长度为32位的“文件”
          3、不是真正文件，类似文件
***********************************************************************/

uint16 GetDirSecCount(void)
{
	return BPB_RootEntCnt * 32 / BPB_BytesPerSec;
}

/***********************************************************************
函数功能：根目录所在第一个簇的簇号，FAT32有效
操作内容：1、FAT32根目录项是不确定的，FAT16为固定值512
          2、根目录项不限制
***********************************************************************/
/*uint32 GetDirStartClus(void)
{
	return BPB_RootClus;
}
*/
/***********************************************************************
函数功能：获取一个给定的簇的扇区开始地址
操作内容：1、计算簇开始扇区号    
          2、它是针对FAT卷扇区0的偏移量
          3、参数ClusID为给定簇号，为2^n值，如2，4，8，16等
***********************************************************************/

uint32 ClusConvLBA(uint16 ClusID)
{
	return DataStartSec() + BPB_SecPerClus * (ClusID - 2);
}

/***********************************************************************
函数功能：判断文件系统类型，FAT16/FAT32中的一种，FAT12目前基本不用了
操作内容：1、实际上文件系统类型的判断只有唯一的方法：判断簇的大小   
          2、FAT12的簇数最大4084，FAT16最大65524，FAT32大于65524
          3、簇数完全由BPB信息确定，可以计算
          4、考虑到FAT32文件系统不存在固定的FAT表区，因此FATSz16=0
          5、通过FATSz16是否为0即可判断是FAT16还是FAT32
***********************************************************************/
void FATSystemType(void)
{ 
  if(BPB_FATSz16!=0)
    FAT32_EN=0;                            //FAT16文件系统
  else 
    FAT32_EN=1;                            //FAT32文件系统
  
  //return FAT32_EN;
/*    FATSz=BPB_FATSz32;
  if(BPB_TotSec16!=0) 
    TotSec=BPB_TotSec16;
  else
    TotSec=BPB_TotSec32;
  DataSec=TotSec-(BPB_RsvdSecCnt+BPB_NumFATs*FATSz)+RootDirSectors);
  CountofClusters=Data / BPB_SecPerClus;  */
}

/***********************************************************************
函数功能：读取文件分配表的指定项，第一个FAT表FAT1
操作内容：1、FAT表位于启动扇区之后，一般是2个   
          2、它用来管理文件簇，每个文件对应一个FAT表
          3、FAT16系统可以管理2^16个文件簇
          4、记录磁盘中有关文件如何被分散存储在不同扇区的信息
          5、每个文件的FAT表占2个字节，FAT32系统是4个字节
***********************************************************************/

uint16 ReadFAT(uint16 Index)
{
	uint16 *RAM = (uint16*)BUFFER;
	ReadBlock(BPB_HiddSec+BPB_RsvdSecCnt + Index / 256);//?????????????
	return RAM[Index % 256];                //求余数以后就是对应扇区内的偏移量，占2个字节
}

/*uint32 Read32FAT(uint32 Index)
{
	uint32 *RAM = (uint32*)BUFFER;
	ReadBlock(BPB_HiddSec + BPB_RsvdSecCnt + Index / 128);
	return RAM[Index % 128];                //求余数以后就是对应扇区内的偏移量，占4个字节
}

*/

/***********************************************************************
函数功能：写文件分配表的指定项，第一个FAT表FAT1
操作内容：1、FAT表位于启动扇区之后，一般是2个   
          2、它用来管理文件簇，每个文件对应一个FAT表
          3、FAT16系统可以管理2^16个文件簇
***********************************************************************/

void WriteFAT(uint16 Index, uint16 Value)
{
	uint16 *RAM = (uint16*)BUFFER;
	uint32 SecID;
	SecID = BPB_HiddSec + BPB_RsvdSecCnt + Index / 256;    //扇区号，将写入FAT值到对应扇区???????????????????????
	ReadBlock(SecID);
	RAM[Index % 256] = Value;                //这是文件对应FAT表的值
	WriteBlock(SecID);
}

/*void Write32FAT(uint32 Index, uint32 Value)
{
        uint32 *RAM = (uint32*)BUFFER;
	uint32 SecID;
	SecID = BPB_HiddSec + BPB_RsvdSecCnt + Index / 128;    //扇区号，将写入FAT值到对应扇区
	ReadBlock(SecID);
	RAM[Index % 128] = Value;                //这是文件对应FAT表的值
	WriteBlock(SecID);
}
*/


/***********************************************************************
函数功能：写文件分配表的指定项，第二个FAT表FAT2
操作内容：1、FAT表位于启动扇区之后，一般是2个   
          2、它用来管理文件簇，每个文件对应一个FAT表
          3、FAT16系统可以管理2^16个文件簇
***********************************************************************/

void WriteFAT2(uint16 Index, uint16 Value)
{
	uint16 *RAM = (uint16*)BUFFER;
	uint32 SecID;
	SecID = BPB_RsvdSecCnt + BPB_FATSz16 + Index / 256;   //第二个FAT表紧跟在第一个FAT表后面
	ReadBlock(SecID);
	RAM[Index % 256] = Value;
	WriteBlock(SecID);
}

/*void Write32FAT2(uint32 Index, uint32 Value)
{
        uint32 *RAM = (uint32*)BUFFER;
	uint32 SecID;
	SecID = BPB_RsvdSecCnt + BPB_FATSz16 + Index / 128;    //扇区号，将写入FAT值到对应扇区
	ReadBlock(SecID);
	RAM[Index % 128] = Value;                              //这是文件对应FAT表的值
	WriteBlock(SecID);
}
*/
/***********************************************************************
函数功能：获取根目录中可以使用的一项
操作内容：1、计算簇开始扇区号    
          2、它是针对FAT卷扇区0的偏移量
          3、参数ClusID为给定簇号，为2^n值，如2，4，8，16等
          4、实际上如果FileName[0]=0x00或者0xE5，均表示为可以使用项，但意义不一样
          5、0x00表示之后都是空，没有目录，可以不再查询；0xE5表示此目录项为空，可以使用
***********************************************************************/

uint16 GetEmptyDIR()
{
	uint16 DirSecCut, DirStart, i, m, ID = 0;
	DirSecCut = 32;               //GetDirSecCount();     //根目录扇区总数，这里限制FAT16和FAT32都最多为512个根目录项
	DirStart = DirStartSec();                                            //根目录开始扇区地址,FAT16为512
	for(i = 0; i < DirSecCut; i++)                                       //在整个扇区空间搜索
	{
		ReadBlock(DirStart + i);                                     //读根目录扇区
		for(m = 0; m < 16; m++)
		{
			if((BUFFER[m * 32] == 0) || (BUFFER[m * 32] == 0xe5))//搜索为0x00区域或者0xe5区域，根目录为32位长度的”文件“
				return ID;                                   //返回可以使用扇区ID
			else
				ID++;
		}
	}
	return ID;                                                            //表示没有空闲可以使用的项
}

/***********************************************************************
函数功能：获取子目录中可以使用的一项
操作内容：1、计算簇开始扇区号    
          2、它是针对父目录所在开始簇的偏移量
          3、参数ClusID为给定簇号，为2^n值，如2，4，8，16等
          4、实际上如果FileName[0]=0x00或者0xE5，均表示为可以使用项，但意义不一样
          5、0x00表示之后都是空，没有目录，可以不再查询；0xE5表示此目录项为空，可以使用
***********************************************************************/

uint16 GetDirectoryEmptyDIR(uint16 ClusID)
{
	uint16 DirSecCut, DirStart, i, m, ID = 0;
	DirSecCut = BPB_SecPerClus*16;                                       //一个文件夹的子文件的目录项，一般一个文件夹占一簇，总项数为扇区数×每扇区的项数
	DirStart = ClusConvLBA(ClusID);                                      //父文件夹的起始簇扇区地址
	for(i = 0; i < DirSecCut; i++)                                       //在整个扇区空间搜索
	{
		ReadBlock(DirStart + i);                                     //读目录扇区
		for(m = 0; m < 16; m++)
		{
			if((BUFFER[m * 32] == 0) || (BUFFER[m * 32] == 0xe5))//搜索为0x00区域或者0xe5区域，根目录为32位长度的”文件“
				return ID;                                   //返回可以使用扇区ID，即偏移量
			else
				ID++;
		}
	}
	return ID;                                                            //表示没有空闲可以使用的项
}

/***********************************************************************
函数功能：获得和文件名对应的根目录项以及该目录的位置
操作内容：1、比较文件名相符   
          2、每个文件都有对应的FAT表
          3、ID的首地址是对应的根目录项的首地址，类型为DIR(见头文件fat.h)
***********************************************************************/

uint8 GetFileID(uint8 Name[11], DIR* ID)
{
	uint16 DirSecCut, DirStart, i, m,Index;
	DirSecCut = 32;                                  //GetDirSecCount();根目录总扇区数，这里将FAT16/32统一到512个根目录项
	DirStart = DirStartSec();                       //根目录开始扇区
	for(i = 0; i < DirSecCut; i++)
	{
		ReadBlock(DirStart + i);                //读取扇区数据
		for(m = 0; m <16; m++)                  //一个扇区里面有16个项，因为每个项的长度是32位
		{
			if(IsEqual(Name, &((DIR*)&BUFFER[m * 32])->FileName, 11))   //进行比较，相等返回index
			{
				*ID = *((DIR*)&BUFFER[m * 32]);   //ID实际就是根目录项
                                Index = i*16+m;                   //FAT16下一般最大是511，也就是说FAT16下根目录项数一般为512，为保证兼容性
				return Index;                     //找到对应的目录项,返回目录的位置.
			}
		}
	}
	return 0;                                         //没有找到对应的目录项,返回0.
}

/*uint8 GetFileID32(uint8 Name[11], DIR32* ID)
{
	uint16 DirSecCut, DirStart, i, m,Index;
	DirSecCut = 32;                                  //GetDirSecCount();根目录总扇区数，这里将FAT16/32统一到512个根目录项
	DirStart = DirStartSec();                       //根目录开始扇区
	for(i = 0; i < DirSecCut; i++)
	{
		ReadBlock(DirStart + i);                //读取扇区数据
		for(m = 0; m <16; m++)                  //一个扇区里面有16个项，因为每个项的长度是32位
		{
			if(IsEqual(Name, &((DIR32*)&BUFFER[m * 32])->FileName, 11))   //进行比较，相等返回1
			{
				*ID = *((DIR32*)&BUFFER[m * 32]);   //ID实际就是根目录项
                                Index = i*16+m;                   //FAT16下一般最大是511，也就是说FAT16下根目录项数一般为512，为保证兼容性
				return Index;                     //找到对应的目录项,返回目录的位置.
			}
		}
	}
	return 0;                                         //没有找到对应的目录项,返回0.
}
*/
/***********************************************************************
函数功能：获取一个空的FAT项
操作内容：1、用来准备写入FAT表    
          2、每个文件都有对应的FAT表
***********************************************************************/

uint16 GetNextFAT(void)
{
	uint16 FAT_Count, i;
	FAT_Count = BPB_FATSz16 * 256;                      //FAT表总项数，一个扇区是256个项，FAT16一个项是2个字节
	for(i = 0; i < FAT_Count; i++)
	{
		if(ReadFAT(i) == 0)                         //为0表示空，可以作为新创建的文件的FAT项
			return i;
	}
	return 0;
}

/*uint32 GetNext32FAT(void)
{
	uint32 FAT_Count, i;
	FAT_Count = BPB_FATSz32 * 128;                      //FAT表总项数，一个扇区是128个项，FAT32一个项是4个字节
	for(i = 0; i < FAT_Count; i++)
	{
		if(Read32FAT(i) == 0)                         //为0表示空，可以作为新创建的文件的FAT项
			return i;
	}
	return 0;
}*/

/***********************************************************************
函数功能：读取根目录的指定项
操作内容：1、读取指定项的根目录数据   
          2、一个项的长度是32位，类似一个“文件”
***********************************************************************/

void ReadDIR(uint16 Index, DIR* Value)
{
	uint32 DirStart = DirStartSec();
	ReadBlock(DirStart + Index / 16);
	CopyBytes(&BUFFER[(Index % 16) * 32], Value, 32);
}


/***********************************************************************
函数功能：写根目录的指定项
操作内容：1、新建文件时，查找到可用根目录项后进行写入   
          2、一个项的长度是32位，类似一个“文件”
***********************************************************************/

void WriteDIR(uint16 Index, DIR* Value)
{
	uint32 LBA = DirStartSec() + Index / 16;
	ReadBlock(LBA);
	CopyBytes(Value, &BUFFER[(Index % 16) * 32], 32);
	WriteBlock(LBA);
}

/*void WriteDIR32(uint16 Index, DIR32* Value)
{
	uint32 LBA = DirStartSec() + Index / 16;
	ReadBlock(LBA);
	CopyBytes(Value, &BUFFER[(Index % 16) * 32], 32);
	WriteBlock(LBA);
}
*/
/***********************************************************************
函数功能：写子目录的“.”和“..”项内容
操作内容：1、一共是2个32Bytes  
          2、一个项的长度是32位，类似一个“文件”
          3、起始地址是子目录的起始簇地址
***********************************************************************/

void WriteDirectoryDIR(uint16 ClusID, uint16 Index, DIR* Value)
{
	uint32 LBA = ClusConvLBA(ClusID);
	ReadBlock(LBA);
	CopyBytes(Value, &BUFFER[(Index % 16) * 32], 32);
	WriteBlock(LBA);
}

/*void WriteDirectoryDIR32(uint16 Index, DIR32* Value)
{
	uint32 LBA = DirStartSec() + Index / 16;
	ReadBlock(LBA);
	CopyBytes(Value, &BUFFER[(Index % 16) * 32], 32);
	WriteBlock(LBA);
}*/

/***********************************************************************
函数功能：文件修改日期转换为对应格式，如2010年07月01日转换为0x3CE1
操作内容：1、修改日期计算公式   
          2、日期数据为2个字节长度
          3、可配合实时时钟使用，这样可以得到实时的更改日期
***********************************************************************/
uint16 GetFileCrtDate(uint16 Year, uint8 Month, uint8 Day)//年、月、日
{
        uint16 FileCrtDate;
        FileCrtDate=(Year-1980)*512+Month*32+Day;    // 0~4位为日(1~31)，5～8位为月(1~12)，9～15位为年(0~127)，相对于1980年1月1日的偏移量
        return FileCrtDate;
}

/***********************************************************************
函数功能：文件修改时间转换为对应格式
操作内容：1、修改时间计算公式   
          2、时间数据为2个字节长度
          3、可配合实时时钟使用，这样可以得到实时的更改时间
***********************************************************************/

uint16 GetFileCrtTime(uint8 Hour, uint8 Min, uint8 Sec)//时、分、秒
{
        uint16 FileCrtTime;
        FileCrtTime=Hour*2048+Min*32+Sec/2;       // 0~4位为秒，2秒为1个单位(0~29)，5～10位为分(0~59)，11～15位为时(0~23)
        return FileCrtTime;
}



/***********************************************************************
函数功能：创建一个子空文件
操作内容：1、文件名必须是数字或者大写字母，小于8位则用空格，扩展名为空格 
          2、文件大小事先不用设置,均为0;
          3、创建子文件的过程实际就是写子目录和FAT表，这里不写文件里的数据
          4、这里文件名为短名，8个字节，也就是8个字符
          5、子文件的目录写在其父目录的起始簇的空白目录项
***********************************************************************/
void CreateDirectoryFile(uint8 DirectoryName[11], uint8 FileName[11])  //父文件夹名和新建文件名，短文件名
{
	uint16 ClusID,ClusIDFile,Index;
        DIR FileDir;
        uint16 FileCrtDateTmp,FileCrtTimeTmp;                 //双字节数据       
        uint8 FileCrtDate[2],FileCrtTime[2];                  //单字节时间日期数据
 
	FileDir.FilePosit.Start = GetNextFAT();               //得到一个空的FAT项，下面用来写对应文件的FAT表       
	ClusID = FileDir.FilePosit.Start;                     //文件簇开始簇号，这是子文件的起始簇地址
        ClusIDFile = FileDir.FilePosit.Start;                 //暂存下来，以备后面写入

	WriteFAT(ClusID, 0xffff);                             //写对应文件的FAT表1，即FAT1,0xffff表示文件结束
        WriteFAT2(ClusID, 0xffff);                            //写对应文件的FAT表2，即FAT2，0xffff表示文件结束     
                             
        Index = GetFileID(DirectoryName,&FileDir);            //搜索得到文件夹的属性，主要是获得起始簇位置，方便写文件的目录项
        ReadDIR(Index, &FileDir);                             //读取父文件夹的目录项，只需要其起始簇
        
        FileCrtDateTmp=GetFileCrtDate(2010,7,2);              //文件修改日期2010年7月2日
        FileCrtDate[0]=(uint8)FileCrtDateTmp&0x00ff;          //低字节提取
        FileCrtDate[1]=(uint8)(FileCrtDateTmp>>8);            //高字节提取       
        FileCrtTimeTmp=GetFileCrtTime(21,20,10);              //文件修改时间21点20分10秒
        FileCrtTime[0]=(uint8)FileCrtTimeTmp&0x00ff;          //低字节提取
        FileCrtTime[1]=(uint8)(FileCrtTimeTmp>>8);            //高字节提取      
        
        CopyBytes(FileName, &FileDir.FileName, 11);           //写入文件名
        
        CopyBytes(FileCrtDate, &FileDir.FileCrtDate, 2);       //写入文件创建日期
        CopyBytes(FileCrtTime, &FileDir.FileCrtTime, 2);       //写入文件创建时间
        
        FileDir.FileAttrib = 0x20;                             //文件属性填充，0x10为子目录属性，即文件夹       
       
        ClusID = FileDir.FilePosit.Start;                      //父文件夹的开始簇号，子文件目录项写入这个簇中
        FileDir.FilePosit.Start=ClusIDFile;                    //新建文件的起始簇
        WriteDirectoryDIR(ClusID,GetDirectoryEmptyDIR(ClusID),&FileDir);//需要扫描父目录的起始簇中空白可以写入子目录的项
        
}


/***********************************************************************
函数功能：在根目录创建一个空文件或文件夹
操作内容：1、FileName文件名必须是数字或者大写字母，扩展名也是如此   
          2、文件大小事先不用设置,均为0;
          3、创建文件的过程实际就是写根目录和FAT表，这里不写文件里的数据
          4、这里文件名为短名，8个字节，也就是8个字符
          5、FileAttrib为属性，0x10为文件夹，0x20为文件
          6、该文件夹只建立于根目录下，可以更改
          7、根目录，没有"."和".."项，如果是子目录，也就是文件夹，则有
***********************************************************************/

void CreateFile(uint8 FileName[11], uint8 FileAttrib)
{
	uint16 ClusID;
        DIR FileDir;
        uint8 DirectoryDIRFileName0[11]=".          ";
        uint8 DirectoryDIRFileName1[11]="..         ";
        
        uint16 FileCrtDateTmp,FileCrtTimeTmp;                 //双字节数据       
        uint8 FileCrtDate[2],FileCrtTime[2];                  //单字节时间日期数据
        
        FileCrtDateTmp=GetFileCrtDate(2012,5,2);              //文件修改日期2010年7月2日
        FileCrtDate[0]=(uint8)FileCrtDateTmp&0x00ff;          //低字节提取
        FileCrtDate[1]=(uint8)(FileCrtDateTmp>>8);            //高字节提取
        
        FileCrtTimeTmp=GetFileCrtTime(21,20,10);              //文件修改时间21点20分10秒
        FileCrtTime[0]=(uint8)FileCrtTimeTmp&0x00ff;          //低字节提取
        FileCrtTime[1]=(uint8)(FileCrtTimeTmp>>8);            //高字节提取
        
        
	//ClusNum = Size / (BPB_SecPerClus * 512) + 1;
	EmptyBytes(&FileDir, sizeof(DIR));                          //先全部填充0
        CopyBytes(FileName, &FileDir.FileName, 11);                 //写入文件名
        
        FileDir.FileAttrib = FileAttrib;                            //文件属性填充，0x10则为文件夹，0x20则为文件
        
        CopyBytes(FileCrtDate, &FileDir.FileCrtDate, 2);            //写入文件创建日期
        CopyBytes(FileCrtTime, &FileDir.FileCrtTime, 2);            //写入文件创建时间
        
	//FileDir.FilePosit.Size = 0;
	FileDir.FilePosit.Start = GetNextFAT();                     //得到一个空的FAT项，下面用来写对应文件的FAT表
        
	ClusID = FileDir.FilePosit.Start;                           //文件簇开始簇号

	WriteFAT(ClusID, 0xffff);                                   //写对应文件的FAT表1，即FAT1,0xffff表示文件结束
        WriteFAT2(ClusID, 0xffff);                                  //写对应文件的FAT表2，即FAT2，0xffff表示文件结束
	WriteDIR(GetEmptyDIR(), &FileDir);                          //写根目录指定项，首先搜索可以使用项，再写入
        
    
  if(FileDir.FileAttrib==0x10)                                      //如果是建立文件夹，则写“.”项和“..项”
     {
        CopyBytes(DirectoryDIRFileName0, &FileDir.FileName, 11);    //写入".项目录内容"，该项为自身目录项内容
        WriteDirectoryDIR(ClusID,0,&FileDir);                       //写入，地址为新建目录的起始簇地址的前2个32Bytes，这项为第0个
        
        CopyBytes(DirectoryDIRFileName1, &FileDir.FileName, 11);    //写入"..项目录内容"，该项为与父目录关系，起始簇地址为父目录的起始簇地址
        FileDir.FilePosit.Start=0x0000;                             //文件夹位于根目录下，可更改为文件夹的父文件夹起始簇地址
        WriteDirectoryDIR(ClusID,1,&FileDir);                       //写入，地址为新建目录的起始簇地址的前2个32Bytes，这项为第1个
     }
}

/*void FAT32CreateFile(uint8 FileName[11])
{
	uint32 ClusID;
        DIR32 FileDir;
        uint16 FileWrDataTmp,FileWrTimeTmp;                         //双字节数据       
        uint8 FileWrData[2],FileWrTime[2];                          //单字节时间日期数据
        
        FileWrDataTmp=GetFileCrtDate(2012,5,2);                     //文件修改日期2010年7月2日
        FileWrData[0]=(uint8)FileWrDataTmp&0x00ff;                  //低字节提取
        FileWrData[1]=(uint8)(FileWrDataTmp>>8);                    //高字节提取
        
        FileWrTimeTmp=GetFileCrtTime(21,20,10);                     //文件修改时间21点20分10秒
        FileWrTime[0]=(uint8)FileWrTimeTmp&0x00ff;                  //低字节提取
        FileWrTime[1]=(uint8)(FileWrTimeTmp>>8);                    //高字节提取
                
	//ClusNum = Size / (BPB_SecPerClus * 512) + 1;
	EmptyBytes(&FileDir, sizeof(DIR));                          //先全部填充0
        CopyBytes(FileName, &FileDir.FileName, 11);                 //写入文件名
        
        FileDir.FileAttrib = 0x20;                                  //文件属性填充
        
        CopyBytes(FileWrData, &FileDir.DIR_WrtDate, 2);             //写入文件修改日期
        CopyBytes(FileWrTime, &FileDir.DIR_WrtTime, 2);             //写入文件修改时间
        
	//FileDir.FilePosit.Size = 0;
	ClusID = GetNext32FAT();                                    //得到一个空的FAT项，下面用来写对应文件的FAT表
        
	FileDir.DIR_FstClusHI=(uint16)(ClusID>>16);                 //文件开始簇高字
        FileDir.DIR_FstClusLo=(uint16)ClusID&0x0000ffff;            //文件簇开始簇号低字

	Write32FAT(ClusID, 0xffffffff);                             //写对应文件的FAT表1，即FAT1,0x0fffffff表示文件结束
        Write32FAT2(ClusID, 0xffffffff);                            //写对应文件的FAT表2，即FAT2,0x0fffffff表示文件结束
	WriteDIR32(GetEmptyDIR(), &FileDir);                        //写根目录指定项，首先搜索可以使用项，再写入
}
*/
/***********************************************************************
函数功能：删除文件
操作内容：1、FileName文件名必须是数字或者大写字母，扩展名也是如此   
          2、先搜索文件目录项及位置，采取比较匹配的方法
          3、文件名的第一个字节写0xE5，表示此目录下无文件
          4、清FAT表，写根目录
***********************************************************************/

void DelFile(uint8 Name[11])
{
      DIR FileDir;
      uint16 ClusID,Index;
      Index = GetFileID(Name,&FileDir);                      //搜索得到文件所在根目录项位置
      FileDir.FileName.NAME[0] = 0xE5;                       //删除根目录下文件
      ClusID = FileDir.FilePosit.Start;                      //文件所在的簇号
      WriteFAT(ClusID,0x0000);                               //清空对应的FAT表FAT1
      WriteFAT2(ClusID,0x0000);                              //使FAT2和FAT1保持一致
      WriteDIR(Index,&FileDir);                              //写指定根目录项
      
}

/*void DelFile32(uint8 Name[11])
{
      DIR32 FileDir;
      uint32 ClusID;
      uint16 Index;
      Index = GetFileID32(Name,&FileDir);                      //搜索得到文件所在根目录项位置
      FileDir.FileName.NAME[0] = 0xE5;                       //删除根目录下文件
      ClusID = ((uint32)FileDir.DIR_FstClusHI)<<16+(uint32)FileDir.DIR_FstClusLo;  //文件所在的簇号
      Write32FAT(ClusID,0x00000000);                                //清空对应的FAT表FAT1
      Write32FAT2(ClusID,0x00000000);                               //使FAT2和FAT1保持一致
      WriteDIR32(Index,&FileDir);                                   //写指定根目录项
      
}*/

/***********************************************************************
函数功能：复制文件分配表,使其和备份一致
操作内容：1、将FAT表FAT1复制到FAT2区域 
          2、FATSz16表示FAT表占用的扇区数，BPB区获取的值
          3、备份FAT表
***********************************************************************/

void CopyFAT(void)
{
	uint16 FATSz, RsvdSecCnt, i;
        if(FAT32_EN==0) 
          FATSz=BPB_FATSz16;
        else
 //         FATSz=BPB_FATSz32;
	RsvdSecCnt = BPB_HiddSec + BPB_RsvdSecCnt;
	for(i = 0; i < FATSz; i++)
	{
		ReadBlock(RsvdSecCnt + i);
		WriteBlock(RsvdSecCnt + FATSz + i);
	}
}

/***********************************************************************
函数功能：FAT16操作文件的数据，支持读写
操作内容：1、Write为1是写操作，为0是读操作，Start为开始字节，Length为操作长度 
          2、Data为操作数据，先写入缓冲区
          3、需要同步更新FAT表，记录文件所占大小位置
***********************************************************************/

void OperateFile(uint8 Write ,uint8 Name[11], uint32 Start, uint32 Length, void* Data)
{
	uint8 *data = Data;
	uint16 BytePerClus, SecPerClus, ClusNum, ClusNext,ClusID,ClusIDValue ,m,Index;
	uint32 LBA, i,length;
	DIR FileDir;
	SecPerClus = BPB_SecPerClus;
	BytePerClus = BPB_SecPerClus * BPB_BytesPerSec;               //每簇的字节数
	Index = GetFileID(Name, &FileDir);                            //文件根目录位置0～511
        length = Length;

	//计算开始位置所在簇的簇号
	ClusNum = Start / BytePerClus;
	ClusID = FileDir.FilePosit.Start;

	for(i = 0; i < ClusNum; i++)
	{
                ClusIDValue = ReadFAT(ClusID);                        //读取对应文件的FAT表，0xFFFF表示文件结束
		WriteFAT(ClusID, 0xffff);
                WriteFAT2(ClusID, 0xffff);
                if(ClusIDValue==0xffff)                               //如过ClusIDValue值不为0xFFFF，其他的表示下一个簇地址
                  ClusNext = GetNextFAT();
                else
                  ClusNext = ClusIDValue; 
                
		WriteFAT(ClusID, ClusNext);
                WriteFAT2(ClusID, ClusNext);
		ClusID = ClusNext;
	}
        WriteFAT(ClusID, 0xffff);
        WriteFAT2(ClusID, 0xffff);

	//计算开始位置所在扇区簇内偏移
	i = (Start % BytePerClus) / 512;

	//计算开始位置扇区内偏移
	m = (Start % BytePerClus) % 512;

	LBA = ClusConvLBA(ClusID) + i;


		for(i = 0; i < SecPerClus; i++)
		{
			if(Write)
				ReadBlock(LBA);
			else
				ReadBlock(LBA++);

			for(m = 0; m < 512; m++)
			{

				if(Write)
					BUFFER[m] = *data++;
				else
                                {*data++ = BUFFER[m];
                                 
                                }       

				//如果读取完成就退出
				if(--Length == 0)
				{
					if(Write)
                                        {
                                                //回写扇区
						WriteBlock(LBA); 
                                               //回写目录
                                                FileDir.FilePosit.Size = Start+length;
                                                WriteDIR(Index,&FileDir);
                                        }
					return;
				}
			}
			if(Write)
				WriteBlock(LBA++); //回写扇区,指针下移
		}
	}

/***********************************************************************
函数功能：FAT32操作文件的数据，支持读写
操作内容：1、Write为1是写操作，为0是读操作，Start为开始字节，Length为操作长度 
          2、Data为操作数据，先写入缓冲区
          3、需要同步更新FAT表，记录文件所占大小位置
***********************************************************************/

/*void Operate32File(uint8 Write ,uint8 Name[11], uint32 Start, uint32 Length, void* Data)
{
	uint8 *data = Data;
	uint16 BytePerClus, SecPerClus, ClusNum ,m,Index;
        uint32 ClusID,ClusIDValue, ClusNext;
	uint32 LBA, i,length;
	DIR32 FileDir;
	SecPerClus = BPB_SecPerClus;
	BytePerClus = BPB_SecPerClus * BPB_BytesPerSec;               //每簇的字节数
	Index = GetFileID32(Name, &FileDir);                            //文件根目录位置0～511
        length = Length;

	//计算开始位置所在簇的簇号
	ClusNum = Start / BytePerClus;
        ClusID=((uint32)FileDir.DIR_FstClusHI<<16)+(uint32)FileDir.DIR_FstClusLo;
  	for(i = 0; i < ClusNum; i++)
	{
                ClusIDValue = Read32FAT(ClusID);                        //读取对应文件的FAT表，0xFFFF表示文件结束
		Write32FAT(ClusID, 0x0fffffff);
                Write32FAT2(ClusID, 0x0fffffff);
                if(ClusIDValue==0x0fffffff)                               //如过ClusIDValue值不为0xFFFF，其他的表示下一个簇地址
                  ClusNext = GetNext32FAT();
                else
                  ClusNext = ClusIDValue; 
                
		Write32FAT(ClusID, ClusNext);
                Write32FAT2(ClusID, ClusNext);
		ClusID = ClusNext;
	}
        Write32FAT(ClusID, 0x0fffffff);
        Write32FAT2(ClusID, 0x0fffffff);

	//计算开始位置所在扇区簇内偏移
	i = (Start % BytePerClus) / 512;

	//计算开始位置扇区内偏移
	m = (Start % BytePerClus) % 512;

	LBA = ClusConvLBA(ClusID) + i;


		for(i = 0; i < SecPerClus; i++)
		{
			if(Write)
				ReadBlock(LBA);
			else
				ReadBlock(LBA++);

			for(m = 0; m < 512; m++)
			{

				if(Write)
					BUFFER[m] = *data++;
				else
					*data++ = BUFFER[m];

				//如果读取完成就退出
				if(--Length == 0)
				{
					if(Write)
                                        {
                                                //回写扇区
						WriteBlock(LBA); 
                                               //回写目录
                                                FileDir.DIR_FileSize = Start+length;
                                                WriteDIR32(Index,&FileDir);
                                        }
					return;
				}
			}
			if(Write)
				WriteBlock(LBA++); //回写扇区,指针下移
		}
	}*/

/***********************************************************************
函数功能：格式化磁盘
操作内容：1、不是真正意义上的格式化，只是清除根目录
          2、实际文件的数据依然存在
***********************************************************************/

/*void FormatCard(void)
{
    uint16 i,DirStart,DirSecNum,FatStart,FatSecNum;
   
    //ReadBPB();

    for(i = 0; i < 512;i++ )BUFFER[i]=0;                  //清除根目录，填入数据0
    DirSecNum = GetDirSecCount();                          //FAT16的根目录项数位512，占32个扇区，FAT32根目录项数不定
    DirStart = DirStartSec();                              //获取根目录开始地址
    for(i=0;i<DirSecNum;i++)                               //按扇区依次填入0
    {
      WriteBlock(DirStart+i);
    }
 

    FatStart = BPB_HiddSec + BPB_RsvdSecCnt;              //清除FAT表
    FatSecNum = BPB_FATSz16;                              //FAT表所占扇区数大小
    for(i=0;i<FatSecNum;i++)
    {
      WriteBlock(FatStart+i);
    }
    WriteFAT(0,0xfff8);                                   //FAT表的第一个字固定为0xfff8
    WriteFAT(1,0xffff);                                   //FAT表的第二个字固定为0xffff

    CopyFAT();                                            //备份FAT表，保持与FAT1一致
    

}*/
       
/***********************************************************************
函数功能：根目录下文件夹和文件数量搜索
操作内容：1、搜索根目录项的目录项数以及属性 
          2、可以判断文件类型，如文件夹、txt文件、BMP文件、MP3文件等
          3、返回文件数量
***********************************************************************/  
void FileCountSearch(void)
{/*
	uint16 BytePerClus, SecPerClus, ClusNum, ClusNext,ClusID,ClusIDValue ,m,Index;
	uint32 LBA, i;
	DIR FileDir;
	SecPerClus = BPB_SecPerClus;
	BytePerClus = BPB_SecPerClus * BPB_BytesPerSec;               //每簇的字节数
	Index = GetFileID(Name, &FileDir);                            //文件根目录位置0～511
  
      */  
        uint16 DirSecCut, DirStart, i, m;//, Count = 0;
	DirSecCut = 32;                                               //根目录扇区总数，这里限制FAT16和FAT32都最多为512个根目录项
	DirStart = DirStartSec();                                     //根目录开始扇区地址,FAT16为512
        DirectoryCount = 0,FileCount = 0;
	for(i = 0; i < DirSecCut; i++)                                //在整个扇区空间搜索
	{
		ReadBlock(DirStart + i);                              //读根目录扇区
		for(m = 0; m < 16; m++)
		{
			if((BUFFER[m * 32] != 0) && (BUFFER[m * 32] != 0xe5))//搜索为0x00区域或者0xe5区域，根目录为32位长度的”文件“
                        {
                          if(BUFFER[m * 32+11] == 0x10)               //0x10为文件夹
                            DirectoryCount++;
                          if(BUFFER[m * 32+11] == 0x20)               //0x20为文件
                            FileCount++;
                        //  else
                            //Count=0;
			}
		}
	}
}
