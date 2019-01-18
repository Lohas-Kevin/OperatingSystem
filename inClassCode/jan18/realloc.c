/* realloc.c (also pointer arithmetic) */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  /* similar: int z[100]; */
  int * z = calloc( 100, sizeof( int ) );

  if ( z == NULL )
  {
    fprintf( stderr, "ERROR: calloc() failed\n" );
    return EXIT_FAILURE;
  }

  z[10] = 1234;

  *(z + 10) = 4567;  /* ==> z + 10 * sizeof( int ) */

  printf( "z[10] is %d\n", *(z + 10) );


  z[2000] = 9999;
  printf( "z[2000] is %d\n", z[2000] );


  free( z );

  return EXIT_SUCCESS;
}
