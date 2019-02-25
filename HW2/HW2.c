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
		
		//create the buffer to read the input
		printf("Enter the command:\n");
		char* rBuffer = (char*)calloc(1024, sizeof(char));
		fgets(rBuffer,1024, stdin);
		
		#ifdef DEBUG1
		printf("You entered: %s", rBuffer);
		printf("length of rBuffer is %ld\n", strlen(rBuffer));
		#endif
		
		
		//if we read Exit
		if(strcmp("Exit\n", rBuffer) == 0){
			printf("goodbye\n");
			//free the memory
			free(rBuffer);
			break;
			
		}	
		
		struct stat sb;
		char* dir = (char*)calloc(1024, sizeof(char));
		getcwd(dir, 1024);
		strcat(dir, "/HW2.c");
		//printf("%s \n", dir);
		lstat("/bin/ls", &sb);
		//printf("%d \n", result);
		
		pid_t pid;
		pid = fork();
		
		
		if(pid == -1){
			perror("fork failed\n");
			return EXIT_FAILURE;
		}
		
		if(pid == 0){
			
			char* argv[] = {"ls", "-l", NULL};
			execv("/bin/ls", argv);
			free(dir);
			free(rBuffer);
			break;
			
		}
		else{
			int status;
			pid_t child_pid = wait(&status);
			printf("[process %d terminated with exit status %d]\n", child_pid, status);
			
		}
		
		
		
		/*
		char* environmentVariable = (char*)calloc(1024, sizeof(char));
		environmentVariable = getenv("$MYPATH");
		
		
		#ifdef DEBUG2
		printf("EV is %s \n", environmentVariable);
		#endif
		*/
		
		
		//free(environmentVariable);
		free(dir);
		free(rBuffer);
	}
	
	
	return EXIT_SUCCESS;
	
}