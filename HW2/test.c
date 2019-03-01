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
	if(getenv("$MYPATH") == NULL){
		free(s);
		printf("$MYPATH is not set\n");
	}else{
		s = getenv("$MYPATH");
		printf("$MYPATH = %s",s);
		free(s);
	}
	
	return EXIT_SUCCESS;
	
}