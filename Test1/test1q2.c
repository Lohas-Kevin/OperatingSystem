#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	
	int x = 150;
	printf("PARENT: x is %d\n", x);
	printf("PARENT: forking...\n");
	pid_t pid = fork();
	printf("PARENT: forked1...\n");
	printf("PARENT: forked2...\n");
	printf("PARENT: forked3...\n");
	printf("PARENT: forked4...\n");
	printf("PARENT: forked5...\n");
	printf("PARENT: forked6...\n");
	
	if(pid == 0){
		
		printf("CHILD: happy birthday\n");
		x *= 2;
		printf("CHILD: %d\n", x);
		
	}
	else{
		
		//wait(NULL);
		printf("PARENT: child completed\n");
		x*= 2;
		printf("PARENT: %d\n", x);
		
	}
	
	return EXIT_SUCCESS;
	
}