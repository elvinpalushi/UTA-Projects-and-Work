/* Coding Assignment 4 */
/* Elvin Palushi - 1002070359 */
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <time.h>

int ReadFileIntoArray(int argc,  char *argv[], int **AP)
{
	FILE *fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		printf("File must be provided on command line...exiting\n");
		exit(1);
	}

	char buffer[1024];
	int counter = 0;
	int i = 0;

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		counter++;
	}

	*AP = malloc(counter * sizeof(int));

	fseek(fp, 0, 0);

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		(*AP)[i] = atoi(buffer);
		i++;
	}

	fclose(fp);

	return counter;
}

void swap(int *SwapA, int *SwapB)
{
    int temp = *SwapA;
    *SwapA = *SwapB;
    *SwapB = temp;
}

int partition (int A[], int low, int high)
{
    int i, j = 0;

    #ifdef QSM
        int middle = (high+low)/2;
        swap(&A[middle], &A[high]);
    #elif QSRND
        int random = (rand() % (high-low+1)) + low;
        swap(&A[random], &A[high]);
    #endif

    int pivot = A[high];

    i = (low - 1);

    for (j = low; j < high; j++)
    {
        if (A[j] < pivot)
        {
            i++;
            swap(&A[i], &A[j]);
        }
    }
    swap(&A[i + 1], &A[high]);

    return (i + 1);
}

void QuickSort(int A[], int low, int high)
{
    if (low < high)
    {
        int ndx = partition(A, low, high);

        QuickSort(A, low, ndx - 1);
        QuickSort(A, ndx + 1, high);
    }
}

void PrintArray(int ArrayToPrint[], int SizeAP)
{
    int i;

    for (i = 0; i < SizeAP; i++)
        printf("%d\n", ArrayToPrint[i]);
}

int main(int argc, char *argv[]) 
{
	int *AP = NULL;

	clock_t start, end, total = 0;

	int elements, i, count;

    if (argc >= 3)
    {
        count = atoi(argv[2]);
    }
    else
    {
        count = 10;
        printf("Number of runs not specified on command line...defaulting to 10\n");
    }

    for (i = 0; i < count; i++)
    {
        elements = ReadFileIntoArray(argc, argv, &AP);

        #ifdef PRINTARRAY
		    PrintArray(AP, elements);
		    printf("\n\n");
	    #endif

        start = clock();
	    QuickSort(AP, 0, elements-1);
	    end = clock();

        total += (end-start);

        printf("Run %d Complete : %ld Tics\n", i+1, end-start);

        #ifdef PRINTARRAY
		    PrintArray(AP, elements);
		    printf("\n\n");
	    #endif

        free(AP);
    }

    printf("The average run time for %d runs is %ld\n", count, total/count);

    printf("\n\nProcessed %d records\n", elements);
	
	return 0; 
} 
