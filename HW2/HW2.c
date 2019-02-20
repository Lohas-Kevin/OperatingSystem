#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>


int main(){
	
	
	while(1){
		printf("Enter the command:\n");
		char* rBuffer = (char*)calloc(1024, sizeof(char));
		fgets(rBuffer,1024, stdin);
		
		#ifdef DEBUG
		printf("You entered: %s", rBuffer);
		printf("length of rBuffer is %ld\n", strlen(rBuffer));
		#endif
		
		if(strcmp("Exit\n", rBuffer) == 0){
			printf("goodbye\n");
			free(rBuffer);
			return EXIT_SUCCESS;
		}
		free(rBuffer);
	}
	
	
}