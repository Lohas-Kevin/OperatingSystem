#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/select.h> 

int clients[32];

typedef struct{
	char** argv;
	int argc;
} argument;

argument* readCommand( char* input ){
	/*
		this function will be used to read the 
		first line of the command
	*/
	char* token;
	
	//create the result
	argument* result = calloc(1, sizeof(argument));
	result -> argv = calloc(10, sizeof(char*));
	result -> argc = 0;
	
	token = strtok(input, " ");
	if(token == NULL){
		free(result -> argv);
		free(result);
		return NULL;
	}
	while( token != NULL ){
		char* temp = calloc( 64, sizeof(char) );
		temp = strcpy(temp, token);
		result->argv[result->argc] = temp;
		result->argc += 1;
		token = strtok( NULL, " " );
	}
	
	return result;
}

void freeArgument( argument* arg ){
	for(int i = 0; i < 10; i++){
		free(arg->argv[i]);
	}
	free(arg->argv);
	free(arg);
}



int commandHandeler( argument* arg ){
	//make sure the input is valid
	if(arg == NULL){
		return -1;
	}
	
	//check the command
	if(strcmp(arg->argv[0], "LOGIN") == 0){
		return 1;
	}
	
	return 0;
}

int checkName( char* name ){
	//we will check the name here
	if(strlen(name) < 4 || strlen(name) > 16){
		return -1;
	}
	for(int i = 0; i < strlen(name); i++){
		if(isalnum( name[i] ) == 0){
			//failed the alnum test
			return -1;
		}
	}
	
	return 0;
}


int main(){
	
	
	char str[] = "           ";
	
	argument* temp = readCommand(str);
	
	printf("the result of CommandHandeler is [%d]\n", commandHandeler(temp) );
	//printf("the result of checkName for MEME is [%d]\n", checkName(temp->argv[1]) );
	
	
	//freeArgument(temp);
}