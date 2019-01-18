/* pass-by-reference.c */

#include <stdio.h>
#include <stdlib.h>

int square( int * x )    /* the * here means we have a pointer */
{
  int q = *x;
/*  return (*x) * (*x); */
  return q * q;
}

int main()
{
  int y = 4;
  return square( &y );   /* & is the address-of operator */
}
