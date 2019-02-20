#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(){
	
	char* a = calloc(7, sizeof(char));
	*a = 'b';
	*(a+1) = 'l';
	*(a+2) = 'a';
	*(a+3) = 'b';
	*(a+4) = 'l';
	*(a+5) = 'a';
	*(a+6) = '\0';
	char* b = a;
	*(b+2) = 'x';
	printf("[%s][%s]\n", a, b);
	free(a);
	
}