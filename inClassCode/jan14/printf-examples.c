/* printf-examples.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
  int i;
  for ( i = 1 ; i < 5 ; i++ )
  {
    printf( "%2d %f %20.15f\n", i, sqrt(i), sqrt(i) );
    printf( "%2d %f %-20.15f\n", i, sqrt(i), sqrt(i) );
    printf( "%-2d %f %-20.50f\n", i, sqrt(i), sqrt(i) );
  }

  char c = 'X';
  char name[16] = "ABCD";
//  char who[16] = "ABCD";

  printf( "%c\n", c );
  printf( "%s\n", name );
  printf( "%c\n", name );
  printf( "%c\n", name[0] );
  printf( "%c\n", name[4] );
  printf( "%c\n", name[16] );
  printf( "%c\n", name[17] );
  printf( "%c\n", name[18] );
  printf( "%c\n", name[19] );
  printf( "%c\n", name[20] );

  return EXIT_SUCCESS;
}
