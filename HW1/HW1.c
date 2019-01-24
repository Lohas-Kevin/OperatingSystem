#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>

int writeStr(char** cache, int pos, char* str){
	/*
		this function takes the cache, the string which will be
		writen into the cache and the position in cache where the 
		string will be writen to.
	*/
	if(*(cache+pos) == NULL){
		//there is no word at pos
		*(cache+pos) = (char*)calloc(strlen(str)+1, sizeof(char));
		strcpy(*(cache+pos), str);
		printf("Word \"%s\" ==> %d (calloc)\n",str,pos);
		return 1;
	}else{
		//there is word exist
		*(cache+pos) = (char*)realloc(*(cache+pos),(strlen(str)+1)*sizeof(char));
		strcpy(*(cache+pos),str);
		printf("Word \"%s\" ==> %d (realloc)\n",str,pos);
		return 1;
	}
	
	return 0;
}


int main( int argc, char ** argv){
	
	//check number of arguments
	if(argc != 3){
		fprintf(stderr, "ERROR: Invalid arguments\n");
		return EXIT_FAILURE;
	}
	
	//check the correctness of cacheLength input(should be pure digits)
	for( int i = 0; i < strlen(*(argv+1)); i++){
		if( isdigit(*(*( argv+1 )+i)) == 0){
			fprintf(stderr,"ERROR: entering non-digit char\n");
			return EXIT_FAILURE;
		}
	}
	int cacheLength = atoi(*(argv+1));
	//check if the cache length is valid or not(should be bigger than 0)
	if(cacheLength <= 0){
		fprintf(stderr, "ERROR: wrong cache size input\n");
		return EXIT_FAILURE;
	}
	
	//create the cache
	char** cache = (char**)calloc(cacheLength, sizeof(char*));
	
#ifdef P1
	printf("the first argument is %s\n", *argv);
	printf("the second argument is %s\n", *(argv+1));
	printf("the third argument is %s\n", *(argv+2));
	printf("the second argument conversion is %d\n", cacheLength);
#endif	

	//open the file and read strings byte by byte
	FILE* fp;
	fp = fopen(*(argv+2), "r");
	if(fp == NULL){
		fprintf(stderr, "ERROR: open file failure\n");
		return EXIT_FAILURE;
	}
	
	int length = 0;
	char* info = (char*) calloc(128, sizeof(char));
	
	while(feof(fp) == 0){
		
		char temp = fgetc(fp);
		if( isalnum(temp) != 0 ){
			//if we are reading a word
			*(info+length) = temp;
			length = length + 1;
		}else if(length >= 3) {
			//if we have read a valid word
			int position = 0;
			for(int i =0; i < length; i++){
				position += (int)*(info+i);
			}
						
		#ifdef D3
			printf("The total ASCII value is: %d\n", position);
		#endif
		
			//calculate the position
			position = position % cacheLength;
			writeStr(cache,position,info);
			
		#ifdef DEBUG2
			printf("The word is %s\n", info);
			printf("Position is %d\n", position);
		#endif	
			
			//free the old read buffer and create new 
			length = 0;
			free(info);
			info = (char*) calloc(128, sizeof(char));
		}else{
			//if we read a word shorter then 3 chars
			length = 0;
			free(info);
			info = (char*) calloc(128, sizeof(char));
		}
		
	
		
	}
	//free memory and close file
	for(int i = 0; i < cacheLength; i++){
		if(*(cache+i) != NULL){
			printf("Cache index %d ==> \"%s\"\n",i,*(cache+i));
		}
		free(*(cache+i));
	}
	free(cache);
	free(info);
	fclose(fp);
	
	



	return EXIT_SUCCESS;
	
	
}