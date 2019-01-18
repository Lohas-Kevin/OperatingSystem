/* dyn.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  char ** cache;   /* array of char* */

  /* dynamically allocate an array of 47 char* elements */
  cache = calloc( 47, sizeof( char * ) );  /* 47x8 bytes allocated */

  if ( cache == NULL )
  {
    fprintf( stderr, "ERROR: calloc() failed.\n" );
    return EXIT_FAILURE;
  }

  /* statically allocate 100 bytes */
                              /*        +---------------------+  */
  char input[100] = "ABCD";   /* input: |ABCD\0...............|  */
                              /*        +---------------------+  */

  printf( "input[73] is '%c'\n", input[73] );

  /* dynamically allocate memory to hold the string "ABCD" */
  cache[3] = calloc( strlen( input ) + 1, sizeof( char ) );

  strcpy( cache[3], input );   /* strcpy() relies on '\0' */

  free( cache[3] );
  free( cache );

  return EXIT_FAILURE;
}
