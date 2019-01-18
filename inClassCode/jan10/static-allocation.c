/* static-allocation.c */

/* compile this code from bash shell (or terminal):

   bash$ gcc -Wall -Werror main.c

   once compiled, execute this code as follows:

   bash$ ./a.out
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int x = 42;    /* x is statically allocated memory
                    (4 bytes allocated on the stack) */

  printf( "sizeof int is %d\n", (int)sizeof( int ) );

  printf( "x is %d\n", x );


  int * y = NULL;   /* y is statically allocated memory
                       that will (hopefully) point to a
                       valid memory address */


  /* use this in the shell/bash to see debugging statements:
     bash$ gcc -Wall -Werror -D DEBUG_MODE main.c
                             ^^^^^^^^^^^^^
   */
#ifdef DEBUG_MODE
  printf( "sizeof int* is %d\n", (int)sizeof( int * ) );
  printf( "sizeof char* is %d\n", (int)sizeof( char * ) );
  printf( "sizeof void* is %d\n", (int)sizeof( void * ) );
#endif

  y = &x;
  printf( "y points to %d\n", *y );

  return EXIT_SUCCESS;   /* 0 */

  /* use "echo $?" in the shell to see the exit status
      of the last-terminated process */

}