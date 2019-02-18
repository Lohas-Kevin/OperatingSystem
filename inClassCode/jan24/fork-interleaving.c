/* fork-interleaving.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  pid_t pid;   /* process id (pid) */

  printf( "before fork()...\n" );

  /* create a new (child) process */
  pid = fork();

  if ( pid == -1 )
  {
    perror( "fork() failed" );
    return EXIT_FAILURE;
  }

  /* fork() will (attempt to) create a new process by
     duplicating/copying the existing running process */

  /* both parent and child processes return here */

  printf( "after fork()...\n" );

  /* delay BOTH parent and child process -- unpredictable
      which one will then be first in the ready queue */
  int i = 0;
  for ( i = 0 ; i < 1000000000; i++ )
   ;

  if ( pid == 0 )
  {
/*    sleep( 30 ); */
    printf( "CHILD: happy birthday!\n" );
  }
  else /* pid > 0 */
  {
/*    sleep( 30 ); */
    printf( "PARENT: my child process ID is %d\n", pid );

    /* to have the parent process wait for the child process
        to complete, we can call wait() or waitpid() */
  }

  printf( "All done\n" );

  return EXIT_SUCCESS;
}
