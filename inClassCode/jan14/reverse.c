/* reverse.c */

/* TO DO:
 * (1) understand this example, in particular what's happening with memory
 * (2) modify to not use bigstring buf -- instead, using calloc()/free()
 */

#include <stdio.h>  /* printf */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, malloc, free */
#include <string.h> /* strlen, strcpy, etc. */

/* MAX is the size of the largest string we can handle */
#define MAX 1000

/* bigstring is a new data type */
typedef char bigstring[MAX];


char * example_string = "this is an\0 example string";


/* reverses a string in place
   returns a pointer to the string       [0] [1] [2] [3] [4] [5]
*/                             /*       +------------------------+ */
char *reverse( char * s )      /* s --> | H | e | l | l | o | \0 | */
{                              /*       +------------------------+ */
  bigstring buf;
  int i, len;
  len = strlen( s );	/* find the length */
  for ( i = 0 ; i < len ; i++ )
  {
     buf[i] = s[len-i-1];
  }
  buf[i] = '\0';	/* null terminate the string! */
  strcpy( s, buf );	/* copy the reversed string from buf back into s */
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
