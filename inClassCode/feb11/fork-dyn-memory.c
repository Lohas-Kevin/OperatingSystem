/* fork-dyn-memory.c */

/* use valgrind to see memory leaks in both parent and child */

/* bash$ valgrind --trace-children=yes a.out */

/* check out the man page for valgrind */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  char * x = malloc( 1024 );

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

    /* no need to call free() before you call exec() */
    execl( "/bin/lwjefwes", "ls", NULL );

    /* still need to call free() if exec() fails */
    perror( "execl() failed" );
    free( x );
    return EXIT_FAILURE;

    /* if you call exit(), still call free() on all dynamically allocated memory */
    exit( EXIT_SUCCESS );
  }
  else /* pid > 0 */
  {
    printf( "PARENT: my child process ID is %d\n", pid );
    free( x );
  }

  printf( "All done.....exiting....\n" );

  return EXIT_SUCCESS;
}
