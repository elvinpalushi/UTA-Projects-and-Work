/* Coding Assignment 1 */
/* Elvin Palushi - 1002070359 */
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <time.h>

typedef struct node
{
	int number;
	struct node *next_ptr;
}
NODE;

void AddNodeToLL(int Number, NODE **LinkedListHead)
{
	NODE *TempPtr, *NewNode;

	NewNode = malloc(sizeof(NODE));
	NewNode->number = Number;
	NewNode->next_ptr = NULL;

	if (*LinkedListHead == NULL)
	{
    	*LinkedListHead = NewNode;
	}
	else
	{
    	TempPtr = *LinkedListHead;

    	while (TempPtr->next_ptr != NULL)
        	TempPtr = TempPtr->next_ptr;

    	TempPtr->next_ptr = NewNode;
	}
}

void ReadFileIntoLL(int argc,  char *argv[], NODE **LLH)
{
	FILE *fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		printf("File must be provided on command line...exiting\n");
		exit(1);
	}

	char buffer[1024];
	NODE *TempPtr;
	int node_count = 0;
	int sum = 0;
	int num = 0;

	TempPtr = *LLH;

	while (fgets(buffer, sizeof(buffer), fp) != NULL)
	{
		num = atoi(buffer);
		AddNodeToLL(num, LLH);

		node_count++;
		sum += num;
	}

	fclose(fp);

	printf("\n%d records were read for a total sum of %d\n", node_count, sum);
}

void PrintLL(NODE *LLH) 
{
	struct node *TempPtr;
	TempPtr = LLH;

	int node_count = 0;
	int sum = 0;

	while (TempPtr != NULL)
	{
		printf("\n%p %d %p\n", TempPtr, TempPtr->number, TempPtr->next_ptr);

		sum += TempPtr->number;

		TempPtr = TempPtr->next_ptr;

		node_count++;
	}

	printf("\n%d records were read for a total sum of %d\n", node_count, sum);
}

void FreeLL(NODE **LLH) 
{
	struct node *TempPtr, *NextNode;
	int node_count = 0;
	int sum = 0;

	TempPtr = *LLH;

	while (TempPtr != NULL)
	{
		#ifdef PRINT
			printf("\nFreeing %p %d %p\n", TempPtr, TempPtr->number, TempPtr->next_ptr);
		#endif

		NextNode = TempPtr->next_ptr;
		sum += TempPtr->number;
		free(TempPtr);
		TempPtr = NextNode;
		node_count++;
	}

	*LLH = NULL;

	printf("\n%d nodes were deleted for a total sum of %d\n", node_count, sum);
}

int main(int argc, char *argv[]) 
{
	NODE *LLH = NULL;

	clock_t start, end;

	start = clock();
	ReadFileIntoLL(argc, argv, &LLH);
	end = clock();

	printf("\n%ld tics to write the file into the linked list\n", end-start);

	#ifdef PRINT
		start = clock();
		PrintLL(LLH);
		end = clock();
		printf("\n%ld tics to print the linked list\n", end-start);
	#endif

	start = clock();
	FreeLL(&LLH);
	end = clock();

	printf("\n%ld tics to free the linked list\n", end-start);
	
	return 0; 
} 
