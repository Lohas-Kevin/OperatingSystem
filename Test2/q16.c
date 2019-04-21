#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>



void * go( void * arg )
{
	char * s = calloc( strlen( arg ) + 1, sizeof( char ) );
	strcpy( s, arg );
	s[6] = '\0';
	fprintf( stderr, "%s", s );
	return NULL;
}


int main()
{
	int a = 1;
	pthread_t tid;
	char * q = "READY FOR THE EXAM";
	pthread_create( &tid, NULL, go, q );
	
	if ( a == 1 ) {
		pthread_join( tid, NULL );
	}
	fprintf( stderr, "%s", q );
	
	if ( a == 2 ){ 
		pthread_join( tid, NULL );
	}
	fprintf( stderr, "!\n" );
	return EXIT_SUCCESS;
}
