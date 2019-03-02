#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>


int checkPath(char* mypath, char* target, char* dic){

	#ifdef DEBUG_MODE2
	fprintf(stderr,"[%s],[%s],[%s]\n", mypath, target, dic);
	#endif
	
	//check whether the path and target are valid
	if(mypath == NULL || target == NULL || dic == NULL){
		
		#ifdef DEBUG_MODE
		fprintf(stderr,"Error: check Path read NULL char* \n");
		fprintf(stderr,"[%s],[%s],[%s]\n", mypath, target, dic);
		#endif
		
		return -1;
	}
	//then check the path from left to right
	else{
		char* token = strtok(mypath, "#");
		while(token != NULL){
			char* temp = calloc(1024, sizeof(char));
			strcpy(temp, token);
			strcat(temp, "/");
			strcat(temp, target);
			
			#ifdef DEBUG_MODE2
			printf("the temp is: %s \n", temp);
			#endif
			
			//check the path
			struct stat sb;
			int result = lstat(temp, &sb);
			
			//if find the path
			if(result == 0 && (sb.st_mode & S_IXUSR)){
				dic = strcpy(dic,temp);
				free(temp);
				
				#ifdef DEBUG_MODE2
				printf("find at: %s\n",dic);
				#endif
				
				return 1;
			}
			//printf("mypath now is:[%s]\n",mypath);
			
			//renew the token and free memory
			token = strtok(NULL,"#");
			free(temp);
		}
		return 0;
	}
}

int divideInput(char* input, char** argv){
	
	if( input == NULL || argv == NULL){
		fprintf(stderr,"Error, DivideInput read NULL data\n");
		fprintf(stderr,"[%s]\n", input);
		return -1;
	}
	
	//then seperate the original input string
	int count = 0;
	char* token = strtok(input, " ");
	
	while( token != NULL){
		char* temp = malloc(strlen(token) + 1);
		strcpy(temp, token);
		argv[count] = temp;
		count++;
		token = strtok(NULL, " ");
	}
	return count;
	
}



int main(){
	
	setvbuf( stdout, NULL, _IONBF, 0 );
	
	int has_pid1 = 0;
	int has_pid2 = 0;
	int bg_pid1 = -1;
	int bg_pid2 = -1;
	int status;
	
	while(1){
		
		//check the background process
		if(has_pid1 == 1){
			pid_t bg1 = waitpid(bg_pid1, &status, WNOHANG);
			if(bg1 > 0){
				printf("[process %d terminated with exit status %d]\n", bg1, status);
				has_pid1 = 0;
				bg_pid1 = -1;
			}
		}
		
		if(has_pid2 == 1){
			pid_t bg2 = waitpid(bg_pid2, &status, WNOHANG);
			if(bg2 > 0){
				printf("[process %d terminated with exit status %d]\n", bg2, status);
				has_pid2 = 0;
				bg_pid2 = -1;
			}
		}
		
		int background = 0;
		int multiTask = 0;
		
		//get the current dir and print command
		char* dir = (char*)calloc(1024, sizeof(char));
		getcwd(dir, 1024);
		printf("%s$ ",dir);
		free(dir);
		
		//read the user inputs
		char* rBuffer = calloc(1024, sizeof(char));
		rBuffer = fgets(rBuffer,1024,stdin);
		rBuffer[strlen(rBuffer)-1] = '\0';
		
		
		//create the variables for input divide
		char** argv = calloc(16, sizeof(char*));
		int count;
		//second set of variables if there is two processes
		char** argv2 = calloc(16, sizeof(char*));
		int count2;
		
		count = divideInput(rBuffer, argv);
		int seppos = -1;
		
		//check is there any background or multi tasks
		if(count >0){
			
			for(int i = 0; i < count; i++){
				//check multitasks
				//printf("[%s]\n", argv[i]);
				if(strcmp(argv[i],"|") == 0){
					multiTask = 1;
					seppos = i;
					
				}
			}
			if( strcmp( argv[count-1], "&" ) == 0 ){
				
				background = 1;
				free(argv[count-1]);
				argv[count-1] = NULL;
				count--;
			}
		}
		
		if(multiTask == 1 && seppos != -1){
			
			for(int i = seppos+1; i < count; i++){
				char* arg2 = calloc(64, sizeof(char*));
				arg2 = strcpy(arg2, argv[i]);
				free(argv[i]);
				argv2[i-seppos-1] = arg2;
				count2++;
			}
			free(argv[seppos]);
			count -= (count2+1);
		}
		
		
		#ifdef DEBUG_MODE
		for(int i = 0; i < count; i++){
			printf("the arg at [%d] is [%s]\n", i, argv[i]);
		}
		#endif
		
		free(rBuffer);
		
		//create the memory block for lstat and execv
		
		int result = -1;
		int result2 = -1;
		char* path = calloc(1024, sizeof(char));
		char* path2 = calloc(1024, sizeof(char));
		char* mypath = calloc(1024, sizeof(char));
		
		
		if(count > 0){
			//if we read Exit
			if(strcmp("exit", argv[0]) == 0){
				printf("bye\n");
		
				//free the memory
				for(int i = 0; i < count; i++){
					free(argv[i]);
				}
				free(argv);
				free(path);
				free(mypath);
				break;
				
			}
			//if we read cd
			else if(strcmp("cd", argv[0]) == 0){
				 
				if(count == 1){
					//if there is no following argument
					chdir(getenv("HOME"));
				}else{
					chdir(argv[1]);
				}
			}
			//if we doesn't read Exit
			else{
				
				//over here we check the input
				if(getenv("MYPATH") == NULL){
					//if $MYPATH is NULL
					//we set $MYPATH to default path
					//Then use the function to check the lstat
					
					mypath = strcpy(mypath, "/bin#.");
					result = checkPath(mypath, argv[0], path);
					
					if(multiTask == 1){
						result2 = checkPath(mypath, argv2[0], path2);
					}
					
				}
				//if $MYPATH is set
				else{
					
					mypath = getenv("MYPATH");
					result = checkPath(mypath, argv[0], path);
					
					if(multiTask == 1){
						result2 = checkPath(mypath, argv2[0], path2);
					}
				}
				
			}
		}
		//after checking the lstat result, start fork
		//if there is a executable found
		
		if(result == 1 ){
			
			int p[2];
			int rc = pipe(p);
			
			if(rc == -1){
				perror("Error: Pipe Failed\n");
			}
			
			
			pid_t pid;
			pid = fork();
			
			//check whether fork success or not
			if(pid == -1){
				perror("Error: fork() failed\n");
				return EXIT_FAILURE;
			}
			
			if(pid == 0){
				//in FIRST Child
				if(multiTask == 1 && result2 == 1){
					//if we have IPC Need
					dup2(p[1], fileno(stdout));
					close(p[0]);
				}else{
					//if we have no IPC needed or the second process is not valid
					close(p[0]);
					close(p[1]);
				}
				execv(path,argv);
			}
			else{
				//in the parent
				//first child wait pid part
				close(p[0]);
				close(p[1]);
				if(background == 0){
					//if process runs foreground
					pid_t child_pid = waitpid(pid, &status, 0);
					printf("[process %d terminated with exit status %d]\n", child_pid, status);
					
				}else{
					//if process runs background
					pid_t child_pid = waitpid(pid, &status, WNOHANG);
					has_pid1 = 1;
					bg_pid1 = pid;
					if(child_pid > 0){
						printf("[process %d terminated with exit status %d]\n", pid, status);
						has_pid1 = 0;
						bg_pid1 = -1;
					}
					
				}
				//if there is the second child, wait pid for that
				if(multiTask == 1 && result2 == 1){
					//if we have valid second process and IPC needed
					printf("bla\n");
					pid_t pid2;
					pid2 = fork();
					
					if(pid2 == -1){
						perror("Error: fork() failed\n");
						return EXIT_FAILURE;
					}
					
					if(pid2 == 0){
						//In Second Child
						dup2(p[0], fileno(stdin));
						close(p[1]);
						execv(path2, argv2);
					}
					else{
						if(background == 0){
							pid_t child_pid = waitpid(pid2, &status, 0);
							printf("[process %d terminated with exit status %d]\n", child_pid, status);
						}else{
							pid_t child_pid = waitpid(pid, &status, WNOHANG);
							has_pid2 = 1;
							bg_pid2 = pid2;
							if(child_pid > 0){
								printf("[process %d terminated with exit status %d]\n", pid2, status);
								has_pid2 = 0;
								bg_pid2 = -1;
							}
							
						}
					}
				}
				
				
			}
		}else{
			fprintf(stderr, "ERROR: command \"%s\" not found\n", argv[0]);
		}
		
		//printf("[%s]\n", rBuffer);
		
		for(int i = 0; i < count; i++){
			free(argv[i]);
		}
		if(multiTask == 1){
			for(int i = 0; i < count2; i++){
				free(argv2[i]);
			}
			
		}
		free(argv2);
		free(argv);
		free(path);
		free(path2);
		free(mypath);
	
	}
	
	
	return EXIT_SUCCESS;
	
}