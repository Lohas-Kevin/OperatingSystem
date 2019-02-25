#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>


int main(){
	
	char* s = calloc(1024, sizeof(char));
	printf("The string s before strcpy is: %s \n", s);
	strcpy(s,"/bin");
	printf("The string s after strcpy is: %s \n", s);
	free(s);
	
}