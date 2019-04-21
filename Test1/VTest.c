#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){
	close(0);
	close(1);
	
	char* s = "VERY_NICE,_HELLO";
	int p[2];
	pipe(p);
	pid_t pid = fork();
	
	if( pid == 0 ){
		write(p[1], s, 16);
		printf("%s\n", s);
	}
	
	else{
		wait(NULL);
		char buffer[1024];
		int rc = read (p[0], buffer, 1024);
		buffer[5] = buffer[13] = buffer[18] = buffer[rc] = '\0';
		//printf("HELLO,_%s\n", buffer+14);
		fprintf(stderr, "HELLO, %s\n", buffer+14);
	}
	/*
	printf("%s\n", s);
	write(p[1], s, 16);
	
	char buffer[1024];
	int rc = read (p[0], buffer, 1024);
	printf("%d\n", rc);
	
	printf("%ld\n", strlen(buffer));
	
	char* t = "\0AAAA\0AAA\0AAAAAA";
	printf("%ld\n", strlen(t));
	*/
	
	return EXIT_SUCCESS;
	
}