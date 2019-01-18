/* fork.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
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
  }
  else /* pid > 0 */
  {
/*    sleep( 1 );  */
    printf( "PARENT: my child process ID is %d\n", pid );
  }

  printf( "All done.....exiting....\n" );

  return EXIT_SUCCESS;
}
