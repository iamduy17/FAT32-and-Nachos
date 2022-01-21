#include "syscall.h"
int main() {
	char c;

	PrintString("Nhap 1 ky tu: ");
	c = ReadChar();
	PrintString("Ky tu vua nhap la: ");
	PrintChar(c);
	PrintChar('\n');
	return 0;
}
