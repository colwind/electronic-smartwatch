
typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;

//uint8 FAT32_EN;                       //FAT文件系统类型，为0则表示FAT16，为1则表示FAT32

#pragma pack(1)

//#if FAT32_EN=0

typedef struct
{
	uint8 BS_jmpBoot[3];       //跳转指令
	uint8 BS_OEMName[8];       //厂商名字
	uint16 BPB_BytesPerSec;    //每个扇区的字节数，一般为512
	uint8 BPB_SecPerClus;      //每簇的扇区数
	uint16 BPB_RsvdSecCnt;     //保留扇区，FAT12/16为1
	uint8 BPB_NumFATs;         //FAT结构数目，一般为2
	uint16 BPB_RootEntCnt;     //根目录项目数，FAT12为224，FAT16为512
	uint16 BPB_TotSec16;       //扇区总数
	uint8 BPB_Media;           //设备类型，如软盘、存储卡，MMC/SD卡为0xF8
	uint16 BPB_FATSz16;        //FAT占用扇区数
	uint16 BPB_SecPerTrk;      //磁道扇区数
	uint16 BPB_NumHeads;       //磁头数
	uint32 BPB_HiddSec;        //隐藏扇区，默认为0
	uint32 BPB_TotSec32;       //如果 BPB_TotSec16 为0，它记录总扇区数
	uint8 BS_DrvNum;           //中断0x13的驱动器参数
	uint8 BS_Reservedl;        //WINDOWS NT使用
	uint8 BS_BootSig;          //扩展引导标记
	uint32 BS_VolID;           //卷标序列号
	uint8 BS_VolLab[11];       //卷标，11个字节
	uint8 BS_FilSysType[8];    //文件系统标志
	uint8 ExecutableCode[448]; //启动代码、数据及填充代码
	uint8 ExecutableMarker[2]; //可启动扇区标志，0x55aa，固定值
} FAT_BPB;

//#else

typedef struct
{
	uint8 BS_jmpBoot[3];       //跳转指令
	uint8 BS_OEMName[8];       //厂商名字
	uint16 BPB_BytesPerSec;    //每个扇区的字节数，一般为512
	uint8 BPB_SecPerClus;      //每簇的扇区数
	uint16 BPB_RsvdSecCnt;     //保留扇区，FAT12/16为1
	uint8 BPB_NumFATs;         //FAT结构数目，一般为2
	uint16 BPB_RootEntCnt;     //根目录项目数，FAT12为224，FAT16为512，FAT32为0
	uint16 BPB_TotSec16;       //扇区总数
	uint8 BPB_Media;           //设备类型，如软盘、存储卡，MMC/SD卡为0xF8
	uint16 BPB_FATSz16;        //FAT占用扇区数
	uint16 BPB_SecPerTrk;      //磁道扇区数
	uint16 BPB_NumHeads;       //磁头数
	uint32 BPB_HiddSec;        //隐藏扇区，默认为0
	uint32 BPB_TotSec32;       //如果 BPB_TotSec16 为0，它记录总扇区数
	uint32 BPB_FATSz32;         //FAT32系统中的一个FAT表所占扇区数
	uint16 BPB_ExtFlags;        //FAT32特有
        uint16 BPB_FSVer;           //FAT32版本号，一般为0
        uint32 BPB_RootClus;        //根目录所在的第一个簇的簇号，通常为2，不是必须为2
        uint16 BPB_FSInfo;          //保留区中FAT32卷FSINFO结构所占的扇区数，一般为1
        uint16 BPB_BkBootSec;       //如果不为0，表示在保留区中引导记录的备份数据所占的扇区数，通常为6，从扇区号6开始
        uint8  BPB_Reserved[12];    //用于将来的扩展，一般填充0
        uint8  BS_DrvNum;           //0x13中断参数
        uint8 BS_Reservedl;        //WINDOWS NT使用
	uint8 BS_BootSig;          //扩展引导标记
	uint32 BS_VolID;           //卷标序列号
	uint8 BS_VolLab[11];       //卷标，11个字节
	uint8 BS_FilSysType[8];    //文件系统标志
	uint8 ExecutableCode[420]; //启动代码、数据及填充代码
	uint8 ExecutableMarker[2]; //可启动扇区标志，0x55aa，固定值
} FAT32_BPB;

typedef struct
{       
        uint32 FSI_LeadSig;         //值为0x41615252，表示该区为FSInfo扇区
        uint8  FSI_Reserved1[480];  //保留扩展用
        uint32 FSI_StrucSig;        //值为0x61417272，更具体的表明该扇区已经使用
        uint32 FSI_Free_Count;      //保存最新的剩余簇数量
        uint32 FSI_Nxt_free;        //设定为驱动程序最后分配出去的簇号，如果为0xFFFFFFFF，则必须从簇2开始查找，查FAT表有用
        uint8  FSI_Reserved2[12];   //保留区
        uint32 FSI_TrailSig;        //值为0xAA550000，结束标记
} FSInfo;
        
//#endif

typedef struct
{
	uint8 NAME[8];                //8字节长度的文件名
	uint8 TYPE[3];                //文件扩展名，3字节长度
} FILE_NAME;
typedef struct
{
	uint16 Start;                 //文件开始簇号
	uint32 Size;                  //文件大小
} FILE_POSIT;
typedef struct
{
	FILE_NAME FileName;           //短文件名，8字节文件名和3字节扩展名，文件名8个字符
	uint8 FileAttrib;             //文件属性，为0x20
	uint8 UnUsed;                  //保留空间，为0
        
        uint8  FileCrtTimeTeenth;      //文件创建时间的毫秒级时间戳，有效值0～199
        uint8  FileCrtTime[2];         //文件创建时间
        uint8  FileCrtDate[2];         //文件创建日期
        uint16 FileLastAccDate;        //最后访问日期
        uint16 FileFstClusHI;          //该目录项簇号的高位字，FAT16此项为0
        
	uint8 FileUpdateTime[2];      //最后一次文件更新时间
	uint8 FileUpdateData[2];      //文件写入日期  0~4位为日(1~31)，5～8位为月(1~12)，9～15位为年(0~127)，相对于1980年1月1日的偏移量
	FILE_POSIT FilePosit;         //文件开始簇及大小
} DIR;                                //FAT16根目录结构

typedef struct
{
	FILE_NAME FileName;            //短文件名，8字节文件名和3字节扩展名，文件名8个字符
	uint8  FileAttrib;             //文件属性，为0x20
	uint8  DIR_NTRes;              //保留给WINDOWS NT使用，设置为0
        uint8  DIR_CrtTimeTeenth;      //文件创建时间的毫秒级时间戳，有效值0～199
        uint16 DIR_CrtTime;            //文件创建时间
        uint16 DIR_CrtDate;            //文件创建日期
        uint16 DIR_LastAccDate;        //最后访问日期
        uint16 DIR_FstClusHI;          //该目录项簇号的高位字，FAT16此项为0
        uint8  DIR_WrtTime[2];         //最后写的时间，文件创建被认为是写
        uint8  DIR_WrtDate[2];         //最后写的日期，文件创建被认为是写
        uint16 DIR_FstClusLo;          //该目录项簇号的低位字
	uint16 DIR_FileSize;           //文件大小，由32位双字组成
} DIR32;                               //FAT32系统文件根目录项内容

typedef struct
{
	uint16 ClusID;                 //开始簇号
	uint16 SecOfClus;              //每簇扇区数
	uint16 ByteOfSec;              //每扇区字节数
} DATA_POSIT;

#pragma pack()


void ReadBlock(uint32 LBA);
void WriteBlock(uint32 LBA);
void CopyBytes(void* S, void* D, uint16 size);
uint8 IsEqual(void* A, void* B, uint8 Size);
void EmptyBytes(void* D, uint16 size);
void FATInit(void);
void ReadBPB(void);
void ReadBPB32(void);
uint32 FATStartSec(void);
uint32 DirStartSec(void);
uint32 DataStartSec(void);
uint16 GetDirSecCount(void);
uint32 GetDirSartClus(void);
uint32 ClusConvLBA(uint16 ClusID);
uint16 ReadFAT(uint16 Index);
void WriteFAT(uint16 Index, uint16 Value);
void WriteFAT2(uint16 Index, uint16 Value);
uint16 GetEmptyDIR(void);
uint8 GetFileID(uint8 Name[11], DIR* ID);
uint16 GetNextFAT(void);
void ReadDIR(uint16 Index, DIR* Value);
void WriteDIR(uint16 Index, DIR* Value);
void CreateFile(uint8 FileName[11],uint8 FileAttrib);
void FAT32CreateFile(uint8 FileName[11]);

//void CreateDirectoryFile(uint8 FileName[11]);
void CreateDirectoryFile(uint8 DirectoryName[11], uint8 FileName[11]);
void CopyFAT(void);
void OperateFile(uint8 Write ,uint8 Name[11], uint32 Start, uint32 Length, void* Data);
void DelFile(uint8 Name[11]);
void FormatCard(void);
void FATSystemType(void);
