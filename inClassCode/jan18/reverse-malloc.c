/* reverse-malloc.c */

#include <stdio.h>  /* printf */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, malloc, free */
#include <string.h> /* strlen, strcpy, etc. */


char * example_string = "this is an\0 example string";


/* reverses a string in place
   returns a pointer to the string       [0] [1] [2] [3] [4] [5]
*/                             /*       +------------------------+ */
char *reverse( char * s )      /* s --> | H | e | l | l | o | \0 | */
{                              /*       +------------------------+ */
  char * buf;
  int i, len;
  len = strlen( s );	/* find the length */

  buf = malloc( len + 1 );   /* for "Hello" we would allocate 6 bytes */

  if ( buf == NULL )
  {
    fprintf( stderr, "ERROR: malloc() failed\n" );
    exit( EXIT_FAILURE );
  }

  for ( i = 0 ; i < len ; i++ )
  {
     buf[i] = s[len-i-1];
  }
  buf[i] = '\0';	/* null terminate the string! */

  strcpy( s, buf );	/* copy the reversed string from buf back into s */

  free( buf );   /* call free() at the earliest possible point in your code */

  return s;
}

int main( int argc, char* argv[] )
{
  printf( "The example string is: %s\n", example_string );

  if ( argc < 2 )
  {
    fprintf( stderr, "Invalid usage - must supply a string as argv[1]\n" );
    return EXIT_FAILURE;
  }

  printf( "%s\n", reverse( argv[1] ) );

  return EXIT_SUCCESS;
}
