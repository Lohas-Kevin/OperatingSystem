/* fd-open-read.c */

/**
 *
 *  file descriptor (fd)
 *
 *  -- each process has a file descriptor table
 *      associated with it that keeps track of its
 *       open files (i.e., byte streams)
 *
 *  fd        C++   Java        C
 *  0 stdin   cin   System.in   scanf(), read(), getchar(), ...
 *  1 stdout  cout  System.out  printf(), write(), putchar(), ...
 *  2 stderr  cerr  System.err  fprintf( stderr, "ERROR: ....\n" );
 *                              perror( "open() failed" );
 *
 *    stdout and stderr (by default) are both displayed on the terminal
 *
 *    stdout is line-buffered when output to the terminal
 *
 *    stdout is block-buffered when output is redirected to an output file
 *
 *    stderr (fd 2) is unbuffered
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  char * name = "testfile.txt";

  /* fd table:
      0 stdin
      1 stdout
      2 stderr
   */

  int fd = open( name, O_RDONLY );

  /* fd table:
      0 stdin
      1 stdout
      2 stderr
      3 testfile.txt (O_RDONLY)
   */


  if ( fd == -1 )
  {
/*    fprintf( stderr, "ERROR: open() failed\n" ); */
    perror( "open() failed" );
    return EXIT_FAILURE;
  }

  printf( "fd is %d\n", fd );  /* this should be fd 3 */

  int j = 5;

while ( j > 0 )
{
  /* TO DO: try changing this to int buffer[20] and
             printing out int values */

  char buffer[21];  /* (static) allocate 1 extra byte for '\0' */
  int rc = read( fd, buffer, 20 );

  if ( rc == -1 )
  {
    perror( "read() failed" );
    return EXIT_FAILURE;
  }

  buffer[rc] = '\0';   /* assume the input data is a string */
  printf( "read %d bytes: \"%s\"\n", rc, buffer );

  j--;
}

  close( fd );

  return EXIT_SUCCESS;
}
