/* buffering.c */

#include <stdio.h>
#include <stdlib.h>

/* when printing to the terminal, a '\n' character will "flush" the
 * buffer, i.e., output everything that has been buffered thus far
 * (this is line-based buffering)
 *
 * when outputing to a file, a '\n' does NOT flush the buffer
 * (this is block buffering)
 */

int main( int argc, char * argv[] )
{
  /* change stdout buffering behavior to be unbuffered */
  setvbuf( stdout, NULL, _IONBF, 0 );

  printf( "HERE0" );   /* without '\n' on the end of this line,
			  the "HERE0" string is buffered, meaning
			  we do not see it in the terminal output */
  
  printf( "argv[0] is %s", argv[0] );

  fprintf( stderr, "HERE8" );   /* stderr is never buffered (i.e., unbuffered) */

#if 0
  fflush( stdout );   /* flush the stdout buffer, i.e.,
			  output everything that has been buffered thus far */
#endif

  int * xyz = NULL;

  printf( "HERE1" );
  *xyz = 1234;

  printf( "HERE2" );
  printf( "xyz points to %d\n", *xyz );

  printf( "HERE3" );
  return EXIT_SUCCESS;
}
