
typedef unsigned char uint8;
typedef unsigned int uint16;
typedef unsigned long uint32;

//uint8 FAT32_EN;                       //FAT�ļ�ϵͳ���ͣ�Ϊ0���ʾFAT16��Ϊ1���ʾFAT32

#pragma pack(1)

//#if FAT32_EN=0

typedef struct
{
	uint8 BS_jmpBoot[3];       //��תָ��
	uint8 BS_OEMName[8];       //��������
	uint16 BPB_BytesPerSec;    //ÿ���������ֽ�����һ��Ϊ512
	uint8 BPB_SecPerClus;      //ÿ�ص�������
	uint16 BPB_RsvdSecCnt;     //����������FAT12/16Ϊ1
	uint8 BPB_NumFATs;         //FAT�ṹ��Ŀ��һ��Ϊ2
	uint16 BPB_RootEntCnt;     //��Ŀ¼��Ŀ����FAT12Ϊ224��FAT16Ϊ512
	uint16 BPB_TotSec16;       //��������
	uint8 BPB_Media;           //�豸���ͣ������̡��洢����MMC/SD��Ϊ0xF8
	uint16 BPB_FATSz16;        //FATռ��������
	uint16 BPB_SecPerTrk;      //�ŵ�������
	uint16 BPB_NumHeads;       //��ͷ��
	uint32 BPB_HiddSec;        //����������Ĭ��Ϊ0
	uint32 BPB_TotSec32;       //��� BPB_TotSec16 Ϊ0������¼��������
	uint8 BS_DrvNum;           //�ж�0x13������������
	uint8 BS_Reservedl;        //WINDOWS NTʹ��
	uint8 BS_BootSig;          //��չ�������
	uint32 BS_VolID;           //�������к�
	uint8 BS_VolLab[11];       //���꣬11���ֽ�
	uint8 BS_FilSysType[8];    //�ļ�ϵͳ��־
	uint8 ExecutableCode[448]; //�������롢���ݼ�������
	uint8 ExecutableMarker[2]; //������������־��0x55aa���̶�ֵ
} FAT_BPB;

//#else

typedef struct
{
	uint8 BS_jmpBoot[3];       //��תָ��
	uint8 BS_OEMName[8];       //��������
	uint16 BPB_BytesPerSec;    //ÿ���������ֽ�����һ��Ϊ512
	uint8 BPB_SecPerClus;      //ÿ�ص�������
	uint16 BPB_RsvdSecCnt;     //����������FAT12/16Ϊ1
	uint8 BPB_NumFATs;         //FAT�ṹ��Ŀ��һ��Ϊ2
	uint16 BPB_RootEntCnt;     //��Ŀ¼��Ŀ����FAT12Ϊ224��FAT16Ϊ512��FAT32Ϊ0
	uint16 BPB_TotSec16;       //��������
	uint8 BPB_Media;           //�豸���ͣ������̡��洢����MMC/SD��Ϊ0xF8
	uint16 BPB_FATSz16;        //FATռ��������
	uint16 BPB_SecPerTrk;      //�ŵ�������
	uint16 BPB_NumHeads;       //��ͷ��
	uint32 BPB_HiddSec;        //����������Ĭ��Ϊ0
	uint32 BPB_TotSec32;       //��� BPB_TotSec16 Ϊ0������¼��������
	uint32 BPB_FATSz32;         //FAT32ϵͳ�е�һ��FAT����ռ������
	uint16 BPB_ExtFlags;        //FAT32����
        uint16 BPB_FSVer;           //FAT32�汾�ţ�һ��Ϊ0
        uint32 BPB_RootClus;        //��Ŀ¼���ڵĵ�һ���صĴغţ�ͨ��Ϊ2�����Ǳ���Ϊ2
        uint16 BPB_FSInfo;          //��������FAT32��FSINFO�ṹ��ռ����������һ��Ϊ1
        uint16 BPB_BkBootSec;       //�����Ϊ0����ʾ�ڱ�������������¼�ı���������ռ����������ͨ��Ϊ6����������6��ʼ
        uint8  BPB_Reserved[12];    //���ڽ�������չ��һ�����0
        uint8  BS_DrvNum;           //0x13�жϲ���
        uint8 BS_Reservedl;        //WINDOWS NTʹ��
	uint8 BS_BootSig;          //��չ�������
	uint32 BS_VolID;           //�������к�
	uint8 BS_VolLab[11];       //���꣬11���ֽ�
	uint8 BS_FilSysType[8];    //�ļ�ϵͳ��־
	uint8 ExecutableCode[420]; //�������롢���ݼ�������
	uint8 ExecutableMarker[2]; //������������־��0x55aa���̶�ֵ
} FAT32_BPB;

typedef struct
{       
        uint32 FSI_LeadSig;         //ֵΪ0x41615252����ʾ����ΪFSInfo����
        uint8  FSI_Reserved1[480];  //������չ��
        uint32 FSI_StrucSig;        //ֵΪ0x61417272��������ı����������Ѿ�ʹ��
        uint32 FSI_Free_Count;      //�������µ�ʣ�������
        uint32 FSI_Nxt_free;        //�趨Ϊ���������������ȥ�Ĵغţ����Ϊ0xFFFFFFFF�������Ӵ�2��ʼ���ң���FAT������
        uint8  FSI_Reserved2[12];   //������
        uint32 FSI_TrailSig;        //ֵΪ0xAA550000���������
} FSInfo;
        
//#endif

typedef struct
{
	uint8 NAME[8];                //8�ֽڳ��ȵ��ļ���
	uint8 TYPE[3];                //�ļ���չ����3�ֽڳ���
} FILE_NAME;
typedef struct
{
	uint16 Start;                 //�ļ���ʼ�غ�
	uint32 Size;                  //�ļ���С
} FILE_POSIT;
typedef struct
{
	FILE_NAME FileName;           //���ļ�����8�ֽ��ļ�����3�ֽ���չ�����ļ���8���ַ�
	uint8 FileAttrib;             //�ļ����ԣ�Ϊ0x20
	uint8 UnUsed;                  //�����ռ䣬Ϊ0
        
        uint8  FileCrtTimeTeenth;      //�ļ�����ʱ��ĺ��뼶ʱ�������Чֵ0��199
        uint8  FileCrtTime[2];         //�ļ�����ʱ��
        uint8  FileCrtDate[2];         //�ļ���������
        uint16 FileLastAccDate;        //����������
        uint16 FileFstClusHI;          //��Ŀ¼��غŵĸ�λ�֣�FAT16����Ϊ0
        
	uint8 FileUpdateTime[2];      //���һ���ļ�����ʱ��
	uint8 FileUpdateData[2];      //�ļ�д������  0~4λΪ��(1~31)��5��8λΪ��(1~12)��9��15λΪ��(0~127)�������1980��1��1�յ�ƫ����
	FILE_POSIT FilePosit;         //�ļ���ʼ�ؼ���С
} DIR;                                //FAT16��Ŀ¼�ṹ

typedef struct
{
	FILE_NAME FileName;            //���ļ�����8�ֽ��ļ�����3�ֽ���չ�����ļ���8���ַ�
	uint8  FileAttrib;             //�ļ����ԣ�Ϊ0x20
	uint8  DIR_NTRes;              //������WINDOWS NTʹ�ã�����Ϊ0
        uint8  DIR_CrtTimeTeenth;      //�ļ�����ʱ��ĺ��뼶ʱ�������Чֵ0��199
        uint16 DIR_CrtTime;            //�ļ�����ʱ��
        uint16 DIR_CrtDate;            //�ļ���������
        uint16 DIR_LastAccDate;        //����������
        uint16 DIR_FstClusHI;          //��Ŀ¼��غŵĸ�λ�֣�FAT16����Ϊ0
        uint8  DIR_WrtTime[2];         //���д��ʱ�䣬�ļ���������Ϊ��д
        uint8  DIR_WrtDate[2];         //���д�����ڣ��ļ���������Ϊ��д
        uint16 DIR_FstClusLo;          //��Ŀ¼��غŵĵ�λ��
	uint16 DIR_FileSize;           //�ļ���С����32λ˫�����
} DIR32;                               //FAT32ϵͳ�ļ���Ŀ¼������

typedef struct
{
	uint16 ClusID;                 //��ʼ�غ�
	uint16 SecOfClus;              //ÿ��������
	uint16 ByteOfSec;              //ÿ�����ֽ���
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