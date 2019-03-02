#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int checkPath(char* mypath, char* target, char* dic){
	//check whether the path and target are valid
	fprintf(stderr,"[%s],[%s],[%s]\n", mypath, target, dic);
	if(mypath == NULL || target == NULL || dic == NULL){
		fprintf(stderr,"Error: check Path read NULL char* \n");
		fprintf(stderr,"[%s],[%s],[%s]\n", mypath, target, dic);
		return -1;
	}
	//then check the path from left to right
	else{
		//fprintf(stderr,"[%s],[%s],[%s]\n", mypath, target, dic);
		char* token = strtok(mypath, "#");
		while(token != NULL){
			char* temp = calloc(1024, sizeof(char));
			strcpy(temp, token);
			strcat(temp, "/");
			strcat(temp, target);
			printf("the temp is: %s \n", temp);
			
			//check the path
			struct stat sb;
			int result = lstat(temp, &sb);
			//if find the path
			if(result == 0 && (sb.st_mode & S_IXUSR)){
				dic = strcpy(dic,temp);
				free(temp);
				printf("find at: %s\n",dic);
				return 1;
			}
			token = strtok(NULL,"#");
			free(temp);
		}
		return 0;
	}
}

int main(){
	char* p = calloc(1024, sizeof(char));
	getenv("MYPATH");
	p = getenv("MYPATH");
	printf("%s \n", p);
	char* s = calloc(1024, sizeof(char));
	s = strcpy(s, "ls -l");
	
	char* c = calloc(1024, sizeof(char));
	c = strcpy(c, "meme.exe WOWS XXXX");
	
	char** instr = calloc(16, sizeof(char*));
	int count = 0;
	char* token = strtok(s," ");
	printf("%s\n", token);
	//add each instructions into the array
	while(token != NULL){
		char* temp = malloc(strlen(token) + 1);
		strcpy(temp, token);
		instr[count] = temp;
		count++;
		token = strtok(NULL, " ");
	}
	int result;
	char* dir = calloc(1024, sizeof(char));
	//check the MYPATH env variable
	char* mypath = calloc(1024, sizeof(char));
	//if $MYPATH is not set
	
	if( getenv("$MYPATH") == NULL ){
		mypath = strcpy(mypath, "/bin#.");
		result = checkPath(mypath, instr[0], dir);
		
	}else{
		mypath = getenv("$MYPATH");
		result = checkPath(mypath, instr[0], dir);
	}
	
	
	if(result == 1){
		pid_t pid;
		pid = fork();
		
		//printf("the dir is: %s\n", dir);
		
		if(pid == -1){
			perror("fork failed\n");
			return EXIT_FAILURE;
		}
			
		if(pid == 0){
			//char* argv[] = {"helloWorld.exe",NULL};
			execv(dir,instr);	
		}
		else{
			int status;
			pid_t child_pid = waitpid(pid,&status,0);
			printf("[process %d terminated with exit status %d]\n", child_pid, status);	
		}
	
	}
	
	free(dir);
	free(mypath);
	for(int i = 0; i < count; i++){
		free(instr[i]);
	}
	free(instr);
	
	/*
	
	if(getenv("$MYPATH") == NULL){
		free(s);
		printf("$MYPATH is not set\n");
	}else{
		s = getenv("$MYPATH");
		printf("$MYPATH = %s",s);
		free(s);
	}
	
	*/
	
	free(s);
	free(c);
	//printf("finished\n");
	return EXIT_SUCCESS;
	
}