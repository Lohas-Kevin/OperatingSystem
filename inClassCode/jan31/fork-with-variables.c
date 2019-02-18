/* fork-with-variables.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int x = 5;
  int * y = malloc( sizeof( int ) );
    /* need to call free() in both parent and child */

  /* variables x and y (on the stack) and the 4 bytes allocated on
      the heap get copied to the child process when we call fork() */

  pid_t pid;   /* process id (pid) */

  /* create a new (child) process */
  pid = fork();

  /* fork() will (attempt to) create a new process by
     duplicating/copying the existing running process */

  if ( pid == -1 )
  {
    perror( "fork() failed" );
    return EXIT_FAILURE;
  }

  if ( pid == 0 )
  {
    printf( "CHILD: happy birthday!\n" );
    x += 12;
    printf( "CHILD: x is %d\n", x );
    free( y );
  }
  else /* pid > 0 */
  {
/*    sleep( 1 );  */
    printf( "PARENT: my child process ID is %d\n", pid );
    x += 100;
    printf( "PARENT: x is %d\n", x );
    *y = 1000;
    /* ... */
    free( y );
  }

  printf( "All done.....exiting....\n" );

  return EXIT_SUCCESS;
}
