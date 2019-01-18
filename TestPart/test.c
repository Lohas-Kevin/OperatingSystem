#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(){
	int x[1000];
	printf("the num at 2000 is %i \n", x[2000]);/* this works because c doesn't know the length of array,
													So, basicly you can access memory out of range */
	printf("the num at 1999 is %i \n", x[1999]);
	printf("the num at 1998 is %i \n", x[1998]);
	
}

