#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>


int main(){
	char str[5] = {'m','e','m','e','\0'};
	printf("The length of str is %ld \n", strlen(str));
	printf("The string is %s\n",str);
	
}