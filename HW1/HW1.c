#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>


int main( int argc, char ** argv){
	
	if(argc != 3){
		fprintf(stderr, "ERROR: Invalid arguments\n");
		return EXIT_FAILURE;
	}
	
	//check the correctness of cacheLength
	int cacheLength = atoi(*(argv+1));
	if(cacheLength <= 0){
		fprintf(stderr, "ERROR: wrong cache size input\n");
		return EXIT_FAILURE;
	}
	
#ifdef P1
	printf("the first argument is %s\n", *argv);
	printf("the second argument is %s\n", *(argv+1));
	printf("the third argument is %s\n", *(argv+2));
	printf("the second argument conversion is %d\n", cacheLength);
#endif	

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
			//if we have read a word
			int position = 0;
			for(int i =0; i < length; i++){
				position += (int)*(info+i);
			}
			position = position % cacheLength;
			
#ifdef DEBUG2
			printf("The word is %s\n", info);
			printf("Position is %d\n", position);
#endif	
			length = 0;
			free(info);
			info = (char*) calloc(128, sizeof(char));
		}
		
	
		
	}
	free(info);
	fclose(fp);
	
	



	return EXIT_SUCCESS;
	
	
}