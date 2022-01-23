/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

//int A[1024];	/* size of physical memory; with code, we'll run out of space!*/
//
//int main()
//{
//    int i, j, tmp;
//
//    /* first initialize the array, in reverse sorted order */
//    for (i = 0; i < 1024; i++)		
//        A[i] = 1024 - i;
//
//    /* then sort! */
//    for (i = 0; i < 1023; i++)
//        for (j = i; j < (1023 - i); j++)
//	   if (A[j] > A[j + 1]) {	/* out of order -> need to swap ! */
//	      tmp = A[j];
//	      A[j] = A[j + 1];
//	      A[j + 1] = tmp;
//    	   }
//    Exit(A[0]);		/* and then we're done -- should be 0! */
//}

int main()
{
	int n;
	int a[100];
	int temp;
	int i, j;
	//Nhap so phan tu cua mang
	for (i = 0;;)
	{
		PrintString("Nhap so phan tu: ");
		n = ReadInt();
		if (n > 100) //Kiem tra xem nguoi dung nhap dung khong. Neu khong bat nhap lai cho toi khi nao dung thi dung.
		{
			PrintString("Ban da nhap sai, vui long nhap lai (n <= 100).");
		}
		else
			break;
	}

	//Nhap cac phan tu cua mang thong qua ham ReadInt
	PrintString("Nhap cac phan tu: ");
	for (i = 0; i < n; i++)
	{
		a[i] = ReadInt();
	}

	//Sap xep mang theo thuat toan bubble sort
	for (i = 0; i < n; i++)
		for (j = 0; j < n - 1 - i; j++)
		{
			if (a[j] > a[j + 1])
			{
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}

	//Xuat mang sau khi da sap xep
	PrintString("Mang sau khi da sap xep: ");
	for (i = 0; i < n; i++)
	{
		PrintInt(a[i]);
		PrintChar(' ');
	}
	PrintChar('\n');

	return 0;
}
