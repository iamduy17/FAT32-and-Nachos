#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <windows.h>
#include <fstream>
#include <string.h>
#include <atlstr.h>
#include <fcntl.h>

using namespace std;

struct FAT32 {
    int bytePerSector;          // 	Số byte của một Sector      (B - 2)
    int sectorPerCluster;       // 	Số Sector của một Cluster   (D - 1) SC
    int reservedSector;         // 	Số Sector của BootSector    (E - 2) SB
    int numFat;                 // 	Số bảng FAT                 (10 - 1) NF
    int rdetEntry;              // 	Số Entry của RDET           (11 - 2) SRDET
    int totalSector;            //  Kích thước Volume           (32 - 4) Sv
    int sectorPerFAT;           //  Số Sector của một bảng FAT  (24 - 4) Sf
    int rootCluster;            //  Cluster bắt đầu của RDET    (2C - 4)  
};

struct NTFS {
    int bytesPerSec;            // 	Số byte của một Sector          (B - 2)
    int sectorPerClus;          // 	Số Sector của một Cluster       (D - 1) SC
    int numSecPerTrack;         //  Số sector/track                 (18 - 2)
    int numSide;                //  Số mặt đĩa (head hay side)      (1A - 2)
    int startSecDisk;           //  Sector bắt đầu của ổ đĩa logic  (1C - 4)
    int totalSector;            //  Số Sector của ổ đĩa logic       (28 - 8)
    int startClusterMFT;        //  Cluster bắt đầu của MFT         (30 - 8)
    int startClusterMFTMirror;  //  Cluster bắt đầu của MFT dự phòng(38 - 8)
    int sizeOfMFTEntry;         //  Kích thước của 1 bản ghi trong MFT Entry (40 - 1) đơn vị tính là Byte
};

struct DIRECTORY {
    char Name[256];             // Tên thư mục/ tập tin
    int Attr;                   // Thuộc tính (thường là thư mục/ tập tin)
    int StartCluster;           // Cluster bắt đầu
    int FileSize;               // Kích cỡ (tính theo byte)
    DIRECTORY* next;            // Trỏ đến thư mục/ tập tin tiếp theo
    DIRECTORY* dir;             // Trỏ đến thư mục con
};


void menu();
int hexToDecimal(BYTE* byte, int offset, int count);
int readSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);

//TODO: Cần sửa tên hàm theo kiểu Camel Case (lưng lạc đà)
// Nhớ đổi ở bên 2 file cpp 

/* Phần của Bootsector */
void initBootsector(BYTE* sector, FAT32& fat32);
void readInformationBootSector(FAT32 fat32);

/* Phần của FAT */
void initFAT(int*& FAT, FAT32 fat32, LPCWSTR drive1);
int firstSectorIndexOfCluster(int N, FAT32 fat32);

/* Phần đọc cây thư mục */
DIRECTORY* readDirectory(int firstRecordIndex, int clusIndex, int* entryList, FAT32 fat32, LPCWSTR drive1, string space);
void readContentOfFile(FAT32 fat32, int clusIndex, LPCWSTR drive1, string space);
void freeDirEntries(DIRECTORY* dir);                                                              // ++++ cần sửa tên hàm++++

/* Phần của Partition BootSector NTFS*/
int sizeOfMFTEntry(BYTE* sector, int offset);
void initPartitionBootsector(BYTE* sector, NTFS& _ntfs);
void readInformationPartitionBootSector(NTFS _ntfs);
