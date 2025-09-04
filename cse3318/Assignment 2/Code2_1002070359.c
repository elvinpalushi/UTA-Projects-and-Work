/* Coding Assignment 2 */
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

void InsertionSort(int A[], int n)
{
    int i, key, j;

    for (j = 1; j < n; j++)
    {
        key = A[j];
        i = j - 1;

        while (i >= 0 && A[i] > key)
        {
            A[i + 1] = A[i];
            i = i - 1;
        }
        A[i + 1] = key;
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

	clock_t start, end;

	int elements;

	elements = ReadFileIntoArray(argc, argv, &AP);

	#ifdef PRINTARRAY
		PrintArray(AP, elements);
		printf("\n\n");
	#endif

	start = clock();
	InsertionSort(AP, elements);
	end = clock();

	#ifdef PRINTARRAY
		PrintArray(AP, elements);
		printf("\nProcessed %d records\n", elements);
	#endif

	printf("Insertion Sort = %ld Tics\n", end-start);

	free(AP);
	
	return 0; 
} 
