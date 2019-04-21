#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main()
{
	int rc;
	int p[2];
	rc = pipe( p );
	printf( "%d %d %d\n", getpid(), p[0], p[1] );
	
	
	rc = fork();
	if ( rc == 0 )
	{
		printf("In if rc = %d ", rc);
		rc = write( p[1], "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26 );
		printf("CHILD: RUNNING ");
		sleep(10);
	}
	printf("Out If rc = %d ", rc);
	if ( rc > 0 )
	{
		printf("PARENT: RUNNING ");
		char buffer[70];
		rc = read( p[0], buffer, 8 );
		buffer[rc] = '\0';
		printf( "PARENT: %d %s ", getpid(), buffer );
		sleep(10);
	}
	
	printf( "BYE\n" );
	return EXIT_SUCCESS;
}
 