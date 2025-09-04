#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main()
{
  clock_t start, end;
  double total;

  start = clock();

  printf("Running Benchmark...\n");

  char * ptr = ( char * ) malloc( 65535 );
  free(ptr);

  char * ptr_array[100000];

  long i;
  // Allocate the first 50,000 elements in array
  for ( i = 0; i < 50000; i++ )
  {
    ptr_array[i] = ( char * ) malloc( 100 );
  }

  // Free the first 25,000 elements in array to create fragmentation
  for ( i = 0; i < 25000; i++ )
  {
    free( ptr_array[i] );
  }

  // Allocate the second 50,000 elements in array
  for ( i = 50000; i < 100000; i++ )
  {
    ptr_array[i] = ( char * ) malloc( 100 );
  }

  // Allocate 25,000 elements in array with alternating sizes
  for ( i = 50000; i < 75000; i += 2 )
  {
    ptr_array[i] = ( char * ) malloc( 32 );
    ptr_array[i + 1] = ( char * ) malloc( 128 );
  }

  // Allocate the next 25,000 elements in array with alternating sizes
  for ( i = 75000; i < 100000; i += 2 )
  {
    ptr_array[i] = ( char * ) malloc( 64 );
    ptr_array[i + 1] = ( char * ) malloc( 256 );
  }

  end = clock();

  total = (double)(end - start) / CLOCKS_PER_SEC;

  printf("Time = %ld Tics\n", end - start);
  printf("Time = %f seconds\n", total);

  return 0;
}
