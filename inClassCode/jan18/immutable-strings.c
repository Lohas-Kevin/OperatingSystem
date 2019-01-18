/* immutable-strings.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main()
{
  /* these two arrays are mutable */
  char a[]  = "ABCD";  /* char a[] = { 'A', 'B', 'C', 'D', '\0' }; */
  char b[4] = "QRST";
     /* char b[4]; b[0] = 'Q'; b[1] = 'R'; b[2] = 'S'; b[3] = 'T'; */


  char * x = "David";   /* The string "David" here is immutable */

   /* "David" is not allocated on the stack or the heap; instead,
       it is part of the code -- allocated on the code/data segment,
        which is read-only memory (of the process memory) */

  printf( "Hi, %s\n", x );

#if 0
  x[2] = 'V';    /* this seg-faults (because we're trying to change
                      read-only memory */
#endif

  char * y = malloc( 20 );  /* copy "David" from code segment into heap */
  strcpy( y, x );
  printf( "Hi, %s\n", y );

  strcpy( y, "Edie" );   /* Just like "David", the string "Edie" is stored
                             in read-only memory (code/data segment) */
  sprintf( y, "Edie" );
  printf( "Hi, %s\n", y );

  char * q = "David";
  char * r = "David";
  char * s = "David";
  char * t = "Edie";

  printf( "address of x is %p\n", x );   /* notice the memory addresses  */
  printf( "address of q is %p\n", q );   /*  are the same for all of the */
  printf( "address of r is %p\n", r );   /*   instances of "David"       */
  printf( "address of s is %p\n", s );
  printf( "address of t is %p\n", t );

  return EXIT_SUCCESS;
}
