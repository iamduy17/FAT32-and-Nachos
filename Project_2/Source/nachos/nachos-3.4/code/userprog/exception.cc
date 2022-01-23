// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------
void IncreaseProgramCounter()
{
	// Cau c: Tang gia tri bien program counter
	
	int counter = machine->ReadRegister(PCReg);

	// Vi tri truoc do gan cho vi tri hien tai
   	machine->WriteRegister(PrevPCReg, counter);
	
    	counter = machine->ReadRegister(NextPCReg);

	// Vi tri hien tai gan cho vi tri tiep theo
    	machine->WriteRegister(PCReg, counter);

	// Vi tri tiep theo cong 4 byte
   	machine->WriteRegister(NextPCReg, counter + 4);
}

/* 
Input: - User space address (int) 
 - Limit of buffer (int) 
Output:- Buffer (char*) 
Purpose: Copy buffer from User memory space to System memory space 
*/
char* User2System(int address,int limit) {
	char* buff = NULL;

	buff = new char[limit +1]; //Cap phat 
	if (buff == NULL)
		return buff;

	memset(buff,0,limit+1);

	int c;
	for (int i = 0 ; i < limit ;i++) {
		machine->ReadMem(address + i, 1, &c);
		buff[i] = (char)c;
		if (c == 0)	//Neu gap ki tu ket thuc chuoi thi dung lai.
			break;
	}
	return buff;
}


/* 
Input: - User space address (int) 
 - Limit of buffer (int) 
 - Buffer (char[]) 
Output:- Number of bytes copied (int) 
Purpose: Copy buffer from System memory space to User memory space 
*/
int System2User(int address,int limit,char* buff)
{
	if (limit < 0) 
		return -1;
	if (limit == 0)
		return limit;

	int i = 0, c = 0;
	do {
		c= (int) buff[i];
		machine->WriteMem(address + i,1,c);
		i++;
	}while(i < limit && c != 0);
	return i;
}


void
ExceptionHandler(ExceptionType which)
{
    	int type = machine->ReadRegister(2);
	int i;
	char *buff = NULL;

	// Bien cua readInt	
	int numlen, num, sign, result;

	// Bien cua printInt
	int n, lengthStr, temp;
	const int minInt = -2147483648;
	int len, iIndex;
	char tmp;

	// Bien cua readChar va printChar
	char c;

	// Bien cua readString va printString
	int addr, length, size;

	switch (which) {
		
		case NoException:		// tra quyen dieu khien ve cho HDH
			return;
			break;
		// HDH thong bao loi va goi ham Halt de dung he thong
		case PageFaultException: {
			printf("No valid translation found\n");
			interrupt->Halt();			
			break;
		}
		case ReadOnlyException: {
			printf("Write attempted to page marked 'read-only'\n");
			interrupt->Halt();			
			break;
		}
		case BusErrorException: {
			printf("Translation resulted in an invalid physical address\n");
			interrupt->Halt();			
			break;

		}
		case AddressErrorException: {
			printf("Unaligned reference or one that was beyond the end of the address space\n");
			interrupt->Halt();			
			break;
		}
		case OverflowException: {
			printf("Integer overflow in add or sub\n");
			interrupt->Halt();			
			break;
		}
		case IllegalInstrException: {
			printf("Unimplemented or reserved instr\n");
			interrupt->Halt();			
			break;
		}
		case NumExceptionTypes: {	
			printf("Error\n");
			interrupt->Halt();			
			break;
		}
		case SyscallException:  {
			switch(type) {
				case SC_Halt: {					
					DEBUG('a', "Shutdown, initiated by user program.\n");
					interrupt->Halt();	
					break;
				}
				
				// Viet cac system calls trong file code/userprog/syscal.h
				case SC_Exit:
					break;
				case SC_Exec:
					break;
				case SC_Join:
					break;
				case SC_Create:
					break;
				case SC_Open:
					break;
				case SC_Read:
					break;
				case SC_Write:
					break;
				case SC_Close:
					break;
				case SC_Fork:
					break;
				case SC_Yield:
					break;
				case SC_ReadInt: {
					buff = new char[20];
					
					// doc toi da 20 ki tu va tra ve so ki tu doc duoc
					numlen = gsc->Read(buff,20);
					// So nguyen duoc nhap o dang so duong
					num = 0;
					// Bien bieu dien dau (+): duong hoac (-): am cua so
					sign = 1;
											
					for(i = 0; i < numlen; i++)
					{
						if(i == 0)
						{
							if(buff[i] == '-')
							{
								sign = sign * (-1);
								continue;			
							}
							else if(buff[i] == '+')
							{
								sign = sign * 1;
								continue;			
							}
							else if(buff[i] == ' ')
								continue;
						}
						if((buff[i] >= '0') && (buff[i] <= '9'))
							num = (num * 10) + (buff[i] - '0');
						else 
						{
							num = 0;
							break;
						}
					}
			
					if(num != 0)
						result = num * sign;
					else
						result = num;

					machine->WriteRegister(2,result);
						
					break;
				}
				case SC_PrintInt: {
					n = machine->ReadRegister(4);
					
					// Chuyen so thanh chuoi
					buff = new char[255];
					if (n == 0) {
						buff = "0";
						lengthStr = 1;	// Cap nhat lai length
					}
					else if (n <= minInt) { // Neu n la so nho hon so nho nhat cua kieu int	
						buff = "-2147483648";
						lengthStr = 11;	// Cap nhat lai length
					}
					else {
						sign = 0;
						i = 0, temp = 0;

						// kiem tra n la so duong hay so am, neu am thi chuyen thanh duong
						if (n >= 0) {
							temp = n;
						}
						else {
							temp = n * -1;
							i = 1;
							sign = 1;
						}

						// Chuyen so duong thanh chuoi nhung bi dao nguoc
						while (temp != 0) {
							buff[i] = temp % 10 + 48;
							temp /= 10;
							i++;
						}
						buff[i] = '\0';
						

						// su dung bien len de lay 1 nua do dai cua chuoi
						lengthStr = i;
						len = 0; iIndex = 0;
						if (sign) {
							len = (float)lengthStr / 2.0 + 0.5;
							iIndex = 0;
						}
						else {
							len = lengthStr / 2;
							iIndex = 1;
						}
						i = sign; // gan sign cho i, neu so do la so am thi chua phan tu dau tien ra 

						// dao chuoi de duoc chuoi can tim
						while (i < len) {
							tmp = buff[lengthStr - i - iIndex];
							buff[lengthStr - i - iIndex] = buff[i];
							buff[i] = tmp;
							i++;
						}
						buff[lengthStr] = '\0';
						if (sign)
							buff[0] = '-';
					}

					gsc->Write(buff,lengthStr);
					
					break;
					
				}
				case SC_ReadChar: {
					buff = new char[2];	// khai bao mang str
					gsc->Read(buff,2);		// doc ki tu duoc nhap vao mang mang str 
					
					c = buff[0];		// ki tu dau cua mang str la ki tu char duoc doc vao
					machine->WriteRegister(2,c);
					
					break;
				}
				
				case SC_PrintChar: {
					c = machine->ReadRegister(4);	// lay ki tu bang cach doc thanh ghi $4
					gsc->Write(&c, 1);			// xuat ra man hinh
					break;
				}

				case SC_ReadString: {
					//Lay dia chi ki tu dau tien cua chuoi
					addr = machine->ReadRegister(4);
					//Lay do dai chuoi cho phep
					length = machine->ReadRegister(5);

					buff = new char[length +1]; //Cap phat bo nho dua tren do dai chuoi cho phep
					if (buff == NULL)
						break;
					memset(buff,0,length + 1);	// khoi tao gia tri 0 cho chuoi buff

					size = gsc->Read(buff, length);     //Lay so byte thuc su doc duoc
					System2User(addr, size, buff);  //copy chuoi tu vung nho system space sang user space co do dai la so byte thuc su cua chuoi nguoi dung nhap
					
					break;
				}
				
				case SC_PrintString: {
					addr = machine->ReadRegister(4); //Lay dia chi ki tu dau tien cua chuoi
					buff = User2System(addr, 255); // Copy chuoi tu vung nho user space sang system space
					if (buff == NULL)
						break;

					i = 0;
					while (buff[i] != 0 && buff[i] != '\n')
					{
						gsc->Write(buff + i, 1); //Xuat tung ky tu cua chuoi ra man hinh
						i++;
					}
					buff[i] = '\n'; //Ket thuc chuoi bang ki tu xuong dong.

					gsc->Write(buff + i, 1); //Xuat ki tu xuong dong ra man hinh
					machine->WriteRegister(2, i - 1);
					
					break;
				}			
			}

			// Goi ham tang bien program counter
			IncreaseProgramCounter();
			break;
		}
		
		default: {
			printf("Unexpected user mode exception %d %d\n", which, type);
			ASSERT(FALSE);
			break;
		}	
	}
}
