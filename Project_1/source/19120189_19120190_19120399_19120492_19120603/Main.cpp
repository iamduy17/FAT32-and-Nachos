#include "Source.h"

int main()
{
	int choice = 0;
	string nameDisk;
	FAT32 fat32;
	NTFS _ntfs;
	do {
		menu();
		cout << "Nhap lua chon: ";
		cin >> choice;

		cout << "Nhap ten o dia (1 ki tu): ";
		cin >> nameDisk;


		switch (choice)
		{
		case 1://Thong tin FAT32
		{
			BYTE sector[512];
			string a = "\\\\.\\" + nameDisk + ":";
			CString str(a.c_str());
			CStringW strw(str);
			LPCWSTR drive1 = strw;


			readSector(drive1, 0, sector);
			initBootsector(sector, fat32);

			//Đọc các thông tin được mô tả trong Boot Sector
			cout << "++++++++++++++++++    THONG TIN BOOT SECTOR     ++++++++++++++++++\n" << endl;
			readInformationBootSector(fat32);

			cout << "\n==========  CAY THU MUC  ===========\n" << endl;
			// Đọc cây thư mục				
			struct DIRECTORY* DIR = NULL;
			int numberOfFATEntries = (fat32.sectorPerFAT * fat32.bytePerSector) / 4;
			int* FAT = new int[numberOfFATEntries * sizeof(int)];
			initFAT(FAT, fat32, drive1);
			// hiển thị cây thư mục 
			DIR = readDirectory(0, fat32.rootCluster, FAT, fat32, drive1, " ");

			freeDirEntries(DIR);
			DIR = NULL;
			system("pause");
			system("cls");
			break;
		}
		case 2://Thong tin NTFS
		{
			BYTE sector[512];
			string a = "\\\\.\\" + nameDisk + ":";
			CString str(a.c_str());
			CStringW strw(str);
			LPCWSTR drive2 = strw;

			readSector(drive2, 0, sector);
			initPartitionBootsector(sector, _ntfs);

			//Đọc các thông tin được mô tả trong Partition Boot Sector
			cout << "++++++++++++++++++    THONG TIN PARTITION BOOT SECTOR     ++++++++++++++++++\n" << endl;
			readInformationPartitionBootSector(_ntfs);
			cout << endl;
			break;

		}
		default:
			break;
		}
	} while (choice != 3);

	return 0;
}