#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SNAPCHAT 0
void * wtf( void * arg )
{
	int * f = (int *)arg;
	printf( "%ldA%d\n", pthread_self(), *f );
	fprintf( stderr, "%ldB\n", pthread_self() );
	return NULL;
}
int main()
{
	close( SNAPCHAT );
	printf( "%ldC\n", pthread_self() );
	fprintf( stderr, "%ldD\n", pthread_self() );
	
	int fd = open( "E.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660 );
	printf( "%ldF\n", pthread_self() );
	fprintf( stderr, "%ldG%d\n", pthread_self(), fd );
	pthread_t tid1, tid2;
	
	int rc = pthread_create( &tid1, NULL, wtf, &fd );
	rc = pthread_create( &tid2, NULL, wtf, &rc );
	
	pthread_join( tid1, NULL );
	pthread_join( tid2, NULL );
	
	printf( "%ldH\n", pthread_self() );
	fprintf( stderr, "%ldI\n", pthread_self() );
	
	fflush( NULL );
	close( fd );
	return EXIT_SUCCESS;
}