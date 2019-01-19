#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>



int main(){
	int cacheLength = 10;
	char** cache = (char**)calloc( cacheLength, sizeof(char*) );
	
	char* str = (char*)calloc(128, sizeof(char));
	*str = 'M';
	*(str+1) = 'e';
	*(str+2) = 'm';
	*(str+3) = 'e';
	*(str+4) = '\0';
	*(cache+3) = (char*)calloc(5, sizeof(char));
	strcpy(*(cache+3), str);
	printf("the length at 3 is %ld\n", strlen(*(cache+3)));
	
	char* str2 = "Jinkela";
	*(cache+3) = (char*)realloc(*(cache+3),(strlen(str2)+1)*sizeof(char));
	strcpy(*(cache+3), str2);
	printf("the length at 3 is %ld\n", strlen(*(cache+3)));
	
	for( int i = 0; i < cacheLength; i++ ){
		if(*(cache + i) != NULL){
			printf( "the string at position %d is %s\n", i, *(cache + i) );
		}
		
	}
	free(str);
	free(*(cache+3));
	free(*(cache+2));
	free(*(cache+5));
	free(cache);
	return EXIT_SUCCESS;
}