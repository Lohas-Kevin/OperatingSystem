/* fork-with-exec.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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


  if ( pid == 0 )   /* CHILD */
  {
    printf( "CHILD: happy birthday!\n" );

#if 0
    /* the child process here will execute the ls command */
    /*                argv[0], argv[1]                    */
    execlp( "/bin/ls",   "ls",    "-l",    "owefijweofjiwe", NULL );
#endif

    execlp( "/bin/ls",   "ls",    "-l",    NULL );

#if 0
    /* what would this do...? */
    execlp( "/usr/bin/gcc", "gcc",  "fork-with-wait.c", NULL );
#endif

    /* we should never get here (unless execlp() failed...) */
    perror( "execlp() failed" );
    return EXIT_FAILURE;
  }
  else /* pid > 0 */
  {
    printf( "PARENT: my child process ID is %d\n", pid );

    /* to have the parent process wait for the child process
        to complete, we can call wait() or waitpid() */

    printf( "PARENT: waiting for my child process to terminate....\n" );

    int status;
#if 0
    pid_t child_pid = wait( &status );   /* wait() BLOCKS indefinitely */
    pid_t child_pid = waitpid( pid, &status, 0 );  /* block indefinitely */

    /* block indefinitely and wait on any child process */
    pid_t child_pid = waitpid( -1, &status, 0 );
#endif

    pid_t child_pid;

    while ( 1 )
    {
      child_pid = waitpid( -1, &status, WNOHANG );
      if ( child_pid != 0 ) break;
      printf( "PARENT: child process not terminated yet...\n" );
      sleep( 1 );
    }


    if ( child_pid == -1 )
    {
      perror( "wait() failed" );
      return EXIT_FAILURE;
    }

    printf( "PARENT: child process %d terminated...\n", child_pid );

    if ( WIFSIGNALED( status ) )   /* child process was terminated due to */
    {                              /*  a signal (e.g., seg-fault, etc.)   */
      printf( "...abnormally\n" );
    }
    else
    {
      int exit_status = WEXITSTATUS( status );
      printf( "...successfully with exit status %d\n", exit_status );
    }
  }

  printf( "All done\n" );  /* note that only the parent prints this now */

  return EXIT_SUCCESS;
}
