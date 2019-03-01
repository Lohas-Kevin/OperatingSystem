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
		char* dir = (char*)calloc(1024, sizeof(char));
		getcwd(dir, 1024);
		printf("%s$ Enter the command:\n",dir);
		char* rBuffer = (char*)calloc(1024, sizeof(char));
		fgets(rBuffer,1024, stdin);
		
		
		#ifdef DEBUG1
		printf("You entered: %s", rBuffer);
		printf("length of rBuffer is %ld\n", strlen(rBuffer));
		#endif
		
		
		//if we read Exit
		if(strcmp("exit\n", rBuffer) == 0){
			printf("goodbye\n");
			
			//free the memory
			free(rBuffer);
			break;
			
		}else{
			/*
				over here we check the input
				firstly, check the mypath variable
			*/
			char* mypath = calloc(1024, sizeof(char));
			mypath = getenv("$MYPATH");
			
			//if $MYPATH is not set
			if(strlen(mypath))
			
			
		}	
		
		struct stat sb;
		
		strcat(dir, "/helloWorld10.exe");
		lstat(dir, &sb);

		
		pid_t pid;
		pid = fork();
		
		
		if(pid == -1){
			perror("fork failed\n");
			return EXIT_FAILURE;
		}
		
		if(pid == 0){
			
			char* argv[] = {"helloWorld10.exe",NULL};
			execv(dir, argv);
			
		}
		else{
			int status;
			pid_t child_pid = waitpid(pid,&status,0);
			printf("[process %d terminated with exit status %d]\n", child_pid, status);
			
			
		}
		
		free(dir);
		free(rBuffer);
	}
	
	
	return EXIT_SUCCESS;
	
}