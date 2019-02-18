/* sleep.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  sleep( 3 );
  printf( "I'm awake!\n" );
  return EXIT_SUCCESS;
}
