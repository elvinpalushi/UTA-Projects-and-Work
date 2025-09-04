/* Coding Assignment 3 */
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

void merge(int arr[], int left, int middle, int right)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[middle + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
        arr[k] = L[i];
        i++;
        }
        else
        {
        arr[k] = R[j];
        j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void MergeSort(int arr[], int L, int R)
{
    if (L < R)
    {
        int M = (L+R)/2;

        MergeSort(arr, L, M);
        MergeSort(arr, M+1, R);
        merge(arr, L, M, R);
    }
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

	clock_t start, end, start2, end2;

	int elements;

	elements = ReadFileIntoArray(argc, argv, &AP);

	#ifdef PRINTARRAY
		PrintArray(AP, elements);
		printf("\n\n");
	#endif

    start = clock();
	MergeSort(AP, 0, elements);
	end = clock();

    #ifdef PRINTARRAY
		PrintArray(AP, elements);
        printf("\n\n");
	#endif

    free(AP);

    elements = ReadFileIntoArray(argc, argv, &AP);

	#ifdef PRINTARRAY
		PrintArray(AP, elements);
		printf("\n\n");
	#endif

	start2 = clock();
	InsertionSort(AP, elements);
	end2 = clock();

	#ifdef PRINTARRAY
		PrintArray(AP, elements);
        printf("\n\n");
	#endif

    printf("Processed %d records\n", elements);

    printf("Merge Sort     = %ld Tics\n", end-start);
	printf("Insertion Sort = %ld Tics\n", end2-start2);

	free(AP);
	
	return 0; 
} 
