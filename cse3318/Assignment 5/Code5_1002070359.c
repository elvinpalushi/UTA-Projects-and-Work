/* Coding Assignment 5 */
/* Elvin Palushi - 1002070359 */
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <time.h>
#include <limits.h>

#define MAX 50

typedef struct
{
    char label[6];
    int distance;
	int previous;
    int visited;
}
Vertex;

void addVertex(char label[], Vertex VertexArray[], int *VertexCount)
{
    Vertex NewVertex;
    strcpy(NewVertex.label, label);
    NewVertex.visited = 0;
    NewVertex.previous = -1;
    NewVertex.distance = INT_MAX;
    VertexArray[(*VertexCount)++] = NewVertex;
}

int ReadFile(int argc, char *argv[], int AdjMatrix[][MAX], Vertex VertexArray[], int *VertexCount)
{
	FILE *fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		printf("File must be provided on command line...exiting\n");
		exit(1);
	}

	char buffer[1024];
    char *Token = NULL;
    int row = 0;

    while (fgets(buffer, sizeof(buffer) - 1, fp))
    {
        if (buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0';
        }

        Token = strtok(buffer, ",");
        addVertex(Token, VertexArray, VertexCount);

        int i = 0;
        Token = strtok(NULL, ",");
        
        while (Token != NULL)
        {
            int adjacentVertexIndex = atoi(Token);
            Token = strtok(NULL, ",");

            int weight = atoi(Token);
            Token = strtok(NULL, ",");

            AdjMatrix[row][adjacentVertexIndex] = weight;
            i += 2;
        }
        row++;
    }

	fclose(fp);

	return *VertexCount;
}

void dijkstra(Vertex VertexArray[], int VertexCount, int AdjMatrix[][MAX], int StartVertex)
{
    int CurrentVertex = StartVertex;
    VertexArray[StartVertex].distance = 0;
    VertexArray[StartVertex].previous = -1;
    VertexArray[StartVertex].visited = 1;

    int x, i, SmallestVertexIndex;

    int dofu = 0;
    int cofuv = 0;
    int dofv = 0;

    for (x = 0; x < VertexCount-1; x++)
    {
        for(i = 0; i < VertexCount; i++)
        {
            if (AdjMatrix[CurrentVertex][i] != -1 && !VertexArray[i].visited)
            {
                dofu = VertexArray[CurrentVertex].distance;
                cofuv = AdjMatrix[CurrentVertex][i];
                dofv = VertexArray[i].distance;

                if (dofu + cofuv < dofv)
                {
                    dofv = dofu + cofuv;
                    VertexArray[i].distance = dofv;
                    VertexArray[i].previous = CurrentVertex;
                }
            }
        }
            
        SmallestVertexIndex = -1;
        int SmallestVertex = INT_MAX;

        for(i = 0; i < VertexCount; i++)
        {
            for(i = 0; i < VertexCount; i++)
            {
                if (!VertexArray[i].visited)
                {
                    if (VertexArray[i].distance < SmallestVertex)
                    {
                        SmallestVertex = VertexArray[i].distance;
                        SmallestVertexIndex = i;
                    }
                }
            }
            CurrentVertex = SmallestVertexIndex;
            VertexArray[CurrentVertex].visited = 1;
        }
    }
}

void PrintAdjMatrix(int AdjMatrix[][MAX], int VertexCount)
{
    int i, j;

    printf("\n");
    for(i = 0; i < VertexCount; i++)
    {
        for(j = 0; j < VertexCount; j++)
            printf("%5d\t", AdjMatrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void PrintVertexArray(Vertex VertexArray[], int VertexCount)
{
    int i;

    printf("\n\nI\tL\tD\tP\tV\n");
    for (i = 0; i < VertexCount; i++)
    {
        printf("%d\t%s\t%d\t%d\t%d\n", i, 
        VertexArray[i].label, VertexArray[i].distance, 
        VertexArray[i].previous, VertexArray[i].visited);
    }
    printf("\n");
}

int main(int argc, char *argv[]) 
{
    int AdjMatrix[MAX][MAX];
    Vertex VertexArray[MAX];

    int StartVertex, EndVertex;
    int VertexCount = 0;

    memset(AdjMatrix, -1, MAX*MAX*sizeof(int));
    
    VertexCount = ReadFile(argc, argv, AdjMatrix, VertexArray, &VertexCount);

    #ifdef PRINTIT
        PrintAdjMatrix(AdjMatrix, VertexCount);
    #endif

    char startLabel[6];
    printf("What is the starting vertex? ");
    scanf("%s", startLabel);

    StartVertex = -1;
    for (int i = 0; i < VertexCount; i++) 
    {
        if (strcmp(startLabel, VertexArray[i].label) == 0) 
            StartVertex = i;
    }
    if (StartVertex == -1) 
    {
        printf("Starting vertex %s not found in the graph.\n", startLabel);
    }

    dijkstra(VertexArray, VertexCount, AdjMatrix, StartVertex);

    #ifdef PRINTIT
        PrintVertexArray(VertexArray, VertexCount);
    #endif

    char endLabel[6];
    printf("What is the destination vertex? ");
    scanf("%s", endLabel);

    EndVertex = -1;
    for (int i = 0; i < VertexCount; i++) 
    {
        if (strcmp(endLabel, VertexArray[i].label) == 0) 
            EndVertex = i;
    }
    if (EndVertex == -1) 
    {
        printf("Destination vertex %s not found in the graph.\n", endLabel);
    }

    int destindex = 0;

    while (destindex < VertexCount && strcmp(endLabel, VertexArray[destindex].label) != 0)
    {
        destindex++;
    }

    if (destindex == VertexCount)
    {
        printf("There is no path from %s to %s.\n", startLabel, endLabel);
    } 
    else
    {
        int path[MAX];
        int pathLength = 0;
        int currentVertex = EndVertex;

        while (currentVertex != -1)
        {
            path[pathLength++] = currentVertex;
            currentVertex = VertexArray[currentVertex].previous;
        }

        printf("The path from %s to %s is ", startLabel, endLabel);
        
        for (int i = pathLength - 1; i >= 0; i--) 
        {
            printf("%s", VertexArray[path[i]].label);
            if (i > 0)
            {
                printf("->");
            }
        }

        printf(" and has a length of %d\n", VertexArray[EndVertex].distance);
    }

	return 0;
}