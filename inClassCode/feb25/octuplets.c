/* octuplets.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

/* function prototypes */
int child();
void parent( int children );

int main()
{
  int i, children = 8;

  for ( i = 0 ; i < children ; i++ )
  {
    pid_t pid = fork();

    if ( pid < 0 )
    {
      perror( "fork() failed" );
      return EXIT_FAILURE;
    }

    if ( pid == 0 )
    {
      int rc = child();
      exit( rc );       /* be sure to exit the child process */
    }
  }

  parent( children );

  return EXIT_SUCCESS;
}



/* each child process sleeps for t seconds, */
/* then returns t when it wakes up...       */
int child()
{
  /* try commenting out the srand() function call below */
  srand( time( NULL ) * getpid() * getpid() );

  int t = 10 + ( rand() % 21 );  /* [10,30] */
  printf( "CHILD %d: I'm gonna nap for %d seconds.\n",
          getpid(), t );
  sleep( t );
  printf( "CHILD %d: I'm awake!\n", getpid() );
  return t;
}


void parent( int children )
{
  int status;  /* exit status from each child process */

  pid_t child_pid;

  printf( "PARENT: I'm waiting for my children to wake up.\n" );

  while ( children > 0 )
  {
    /* wait until a child process exits */
    child_pid = wait( &status );   /* BLOCK */

    children--;

    printf( "PARENT: child %d terminated...", child_pid );

    if ( WIFSIGNALED( status ) )
    {
      printf( "abnormally\n" );  /* core dump or kill or kill -9 */
    }
    else if ( WIFEXITED( status ) )
    {
      int rc = WEXITSTATUS( status );
      printf( "successfully with exit status %d\n", rc );
    }

    printf( "PARENT: %d children to go....\n", children );
  }

  printf( "PARENT: All of my children are awake!\n" );
}
