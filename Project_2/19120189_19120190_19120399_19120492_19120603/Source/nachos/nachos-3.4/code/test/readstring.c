#include "syscall.h"

int main()
{
	char a[1000];

	PrintString("Nhap 1 chuoi: ");
	ReadString(a, 1000);
	PrintString("Chuoi vua nhap la: ");
	PrintString(a);

	return 0;
}
