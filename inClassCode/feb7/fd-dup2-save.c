/* fd-dup2.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
  int fd = open( "testfile.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660 );

           /* 0660 ==> 110 110 000    (0660 is octal)
                       rwx rwx rwx
                       ^^^ ^^^ ^^^
                        |   |   |
                        |   |  no permissions for other/world
                        |   |
                        |  rw for group
                        |   
                     rw for file owner */

  if ( fd == -1 )
  {
    perror( "open() failed" );
    return EXIT_FAILURE;
  }

  printf( "fd is %d\n", fd );   /* 3 */

  /* fd table:

     0: stdin
     1: stdout
     2: stderr
     3: testfile.txt O_WRONLY */

  /* save stdout to fd 8 in fd table */
  int rc = dup2( 1, 8 );  /* copy fd 1 into fd 8 */

  rc = dup2( 3, 1 );  /* copy fd 3 into fd 1 */

#if 0
  int rc = dup2( 1, 3 );  /* copy fd 1 into fd 3 */
  write( 3, "weiofjweofiwefi", 12 );
#endif

  /* to do: check the rc for error....! */

  printf( "rc from dup2() is %d\n", rc );

  /* fd table:

     0: stdin
     1: testfile.txt O_WRONLY
     2: stderr
     3: testfile.txt O_WRONLY */

  printf( "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n" );

  /* restore stdout from fd 8 back to fd 1 */
  rc = dup2( 8, 1 );  /* copy fd 8 into fd 1 */
  printf( "hmmmm....\n" );

  close( fd );
  close( 1 );

  return EXIT_SUCCESS;
}
