/* Coding Assignment 6 */
/* Elvin Palushi - 1002070359 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
 
#define HASHTABLESIZE 35
 
/* Node for storing information */
typedef struct OnePiece
{
    char *name;
    float height;
    char gender;
    int age;
    struct OnePiece *next_ptr;
}
ONEPIECE;

/* This function takes care of the newline */
void trim(char *str) 
{
    int l = strlen(str);

    if (str[l-1] == '\n') 
	{
        str[l-1] = '\0';
    }
}
 
/* This function creates an index corresponding to the input key */
int MyHashFunction(char *name) 
{
    int value = 0;

    for (int i = 0; i < strlen(name); i++) 
	{
        value += name[i];
    }

    return value % HASHTABLESIZE;
}

void AddNode(ONEPIECE *NewNode, ONEPIECE *Character[])
{
	int HashIndex = MyHashFunction(NewNode->name);
	
	/* a linked list does not exist for this cell of the array */
	if (Character[HashIndex] == NULL) 
	{
		#if PRINTINSERT
		printf("\nInserting %s at index %d\n", NewNode->name, HashIndex);
		#endif
		Character[HashIndex] = NewNode;
	}
	else   /* A Linked List is present at given index of Hash Table */ 
	{
		ONEPIECE *TempPtr = Character[HashIndex];
	
		/* Traverse linked list */
		while (TempPtr->next_ptr != NULL) 
		{
			TempPtr = TempPtr->next_ptr;
		}
		TempPtr->next_ptr = NewNode;
		#if PRINTINSERT
		printf("\nInserting %s at index %d\n", NewNode->name, HashIndex);
		#endif
	}
}

void FreeDynamicMemory(ONEPIECE *Character[])
{
	ONEPIECE *TempPtr = NULL, *NextPtr = NULL;
	
	for (int i = 0; i < HASHTABLESIZE; i++)
	{
		TempPtr = Character[i];
 
		if (TempPtr != NULL) 
		{
			while (TempPtr != NULL) 
			{
				free(TempPtr->name);
				NextPtr = TempPtr->next_ptr;
				free(TempPtr);
				TempPtr = NextPtr;
			}	
		}
	}
}

/* Remove an element from Hash Table */ 
int DeleteNode(ONEPIECE *Character[])
{
	char LookupName[100] = {};
	int result = 0;
	
	printf("Enter the name of the One Piece Character to delete from the Character Index ");
	fgets(LookupName, sizeof(LookupName), stdin);
	trim(LookupName);

	int HashIndex = MyHashFunction(LookupName);

	/* Get linked list at key in array */
	ONEPIECE *TempPtr = Character[HashIndex];
	ONEPIECE *PrevPtr = NULL;
 
	/* This array index does not have a linked list; therefore, no keys */
	if (TempPtr == NULL) 
	{
		printf("\n\n%s does not exist in the Character Index\n\n", LookupName);
		result = 0;
	}
	else 
	{
		while (TempPtr != NULL) 
		{
			if (strcmp(TempPtr->name, LookupName) == 0)
			{
				/* If the node being deleted is the head node */
				if (TempPtr == Character[HashIndex])
				{
					/* The node the head was pointing at is now the head */
					Character[HashIndex] = TempPtr->next_ptr;
					printf("\n%s has been deleted from the Character Index\n\n", TempPtr->name);
					free(TempPtr);
					TempPtr = NULL;
				}
				/* Found node to be deleted - node is not the head */
				else
				{
					PrevPtr->next_ptr = TempPtr->next_ptr;
					printf("\n%s has been deleted from the Character Index\n\n", TempPtr->name);
					free(TempPtr);
					TempPtr = NULL;
				}
				result = 1;
			}
			/* this is not the node that needs to be deleted but save */
			/* its info and move to the next node in the linked list  */
			else
			{
				PrevPtr = TempPtr;
				TempPtr = TempPtr->next_ptr;
			}
		}
		if (result == 0)
		{
			printf("\n\n%s does not exist in the Character Index\n\n", LookupName);
		}
	}
	return result;
}

/* display the contents of the Hash Table */
void DisplayHashTable(ONEPIECE *Character[])
{
	int i;
	ONEPIECE *TempPtr = NULL;
	
	for (i = 0; i < HASHTABLESIZE; i++) 
	{
		TempPtr = Character[i];

		printf("\nCharacter[%d]-", i);
		
		if (TempPtr != NULL) 
        {
			while (TempPtr != NULL)
			{
				printf("%s|", TempPtr->name);
				TempPtr = TempPtr->next_ptr;
			}
		}
	}
}

void ShowByLetter(ONEPIECE *Character[])
{
	int i;
	ONEPIECE *TempPtr = NULL;
	char LookupLetter = 'A';

	printf("\n\nEnter a letter of the alphabet ");
	scanf(" %c", &LookupLetter);
	LookupLetter = toupper(LookupLetter);

	for (i = 0; i < HASHTABLESIZE; i++) 
	{
		TempPtr = Character[i];

		if (TempPtr != NULL) 
		{
			while (TempPtr != NULL)
			{
				if (toupper(TempPtr->name[0]) == LookupLetter)
				{
					printf("%s\n", TempPtr->name);
				}
				TempPtr = TempPtr->next_ptr;
			}
		}
	}
}

void ShowByName(ONEPIECE *Character[])
{
	ONEPIECE *TempPtr = NULL;
	char height[10] = {};
	char feet[10] = {};
	char inches[10] = {};
	char LookupName[100] = {};
	int FoundIt = 0;
	
	printf("\n\nEnter a One Piece Character's name: ");
    fgets(LookupName, sizeof(LookupName), stdin);
	trim(LookupName);
	
	#if TIMING
	clock_t start, end;
	start = clock();
	#endif
	for (int i = 0; i < HASHTABLESIZE; i++) 
	{
		TempPtr = Character[i];

		if (TempPtr != NULL) 
		{
			while (TempPtr != NULL)
			{
				if (strcmp(TempPtr->name, LookupName) == 0)
				{
					#if TIMING
					end = clock();
					printf("\n\nSearch took %ld tics\n\n", end-start);
					#endif

					FoundIt = 1;
					printf("\n\n%s\n", TempPtr->name);
					
					printf("Height\t\t");
					sprintf(height, "%.1f", TempPtr->height);
					for (int i = 0; i < strlen(height); i++)
					{
						if (height[i] == '.')
						{
							printf("\' ");
						}
						else
						{
							printf("%c", height[i]);
						}
					}
					printf("\"\n");
					if (TempPtr->gender == 'B')
						printf("Gender\t\tM F\n");
					else if (TempPtr->gender == 'U')
						printf("Gender\t\tUnknown\n");
					else
						printf("Gender\t\t%c\n", TempPtr->gender);
					printf("Age\t\t%d\n", TempPtr->age);

				}
				TempPtr = TempPtr->next_ptr;
			}
		}
	}
	
	if (FoundIt == 0)
		printf("\n\n%s not found in Character Index\n\n", LookupName);
}

void AddNewCharacter(ONEPIECE *Character[])
{
	int HashIndex = 0;
	ONEPIECE *NewNode;
	char TempBuffer[100] = {};

	NewNode = malloc(sizeof(ONEPIECE));
	NewNode->next_ptr = NULL;

	printf("\n\nEnter new One Piece Character's name: ");
    fgets(TempBuffer, sizeof(TempBuffer), stdin);
	trim(TempBuffer);

	NewNode->name = malloc(strlen(TempBuffer)*sizeof(char)+1);
	strcpy(NewNode->name, TempBuffer);

	printf("\n\nEnter %s's height as feet.inches ", NewNode->name);
	scanf("%f", &(NewNode->height));
	
	printf("\n\nEnter %s's possible gender (M/F/B/U) ", NewNode->name);
	scanf(" %c", &(NewNode->gender));
	NewNode->gender = toupper(NewNode->gender);
	
	// Extra fgets to clear stdin
	fgets(TempBuffer, sizeof(TempBuffer)-1, stdin);

	printf("\n\nEnter %s's Age ", NewNode->name);
	scanf("%d", &(NewNode->age));

	AddNode(NewNode, Character);
}

int ReadFileIntoHashTable(int argc, char *argv[], ONEPIECE *Character[])
{
	FILE *FH = NULL;
	char FileLine[100] = {};
	char *token = NULL;
	int counter = 0;
	int HashIndex = 0;
	ONEPIECE *NewNode;

	if (argc > 1)
	{
		FH = fopen(argv[1], "r");

		if (FH == NULL)
		{
			perror("Exiting ");
			exit(0);
		}
		
		while (fgets(FileLine, sizeof(FileLine)-1, FH))
		{
			token = strtok(FileLine, "|");

			NewNode = malloc(sizeof(ONEPIECE));
			NewNode->next_ptr = NULL;
			
			NewNode->name = malloc(strlen(token)*sizeof(char)+1);
			strcpy(NewNode->name, token);

			token = strtok(NULL, "|");
			NewNode->height = atof(token);
			
			token = strtok(NULL, "|");
			NewNode->gender = *token;
			
			token = strtok(NULL, "|");
			NewNode->age = atoi(token);

			AddNode(NewNode, Character);

			counter++;
		}

	}
	else
	{
		printf("File must be provided on command line...exiting\n");
		exit(0);
	}
	
	fclose(FH);
	
	return counter;
}

int main(int argc, char *argv[]) 
{
	int MenuChoice = 0;
	int counter = 0;
	ONEPIECE *Character[HASHTABLESIZE] = {};

	enum Menu {SHOWBYLETTER=1, SHOWBYNAME, COUNT, DISPLAY, ADD, DELETE, EXIT};
 
	counter = ReadFileIntoHashTable(argc, argv, Character);
 
	do
	{
		printf("\n\nOne Piece Character Index Menu\n\n"
			   "1. Show all One Piece Characters in the Character Index for a given letter\n"
			   "2. Look up One Piece Characters by name\n"
			   "3. How many One Piece Characters are in the Character Index?\n"
			   "4. Display the Character Index\n"
			   "5. Add a new One Piece Character\n"
			   "6. Delete a One Piece Character from the Character Index\n"
			   "7. Exit\n\n"
			   "Enter menu choice ");
	
		scanf("%d", &MenuChoice);
		getchar();
		printf("\n\n");

		switch (MenuChoice)
		{	
			case SHOWBYLETTER:
				ShowByLetter(Character);
				break;
			case SHOWBYNAME:
				ShowByName(Character);
				break;
			case COUNT:
				printf("Your Character Index contains %d One Piece Characters\n", counter); 
				break;
 			case DISPLAY:
				DisplayHashTable(Character);
				break;
			case ADD:
				AddNewCharacter(Character);
				counter++;
				break;
			case DELETE:
				if (DeleteNode(Character))
				{
					counter--;
				}
				break;
			case EXIT:
				break;
			default : 
				printf("Invalid menu choice\n\n"); 
		}
	}
	while (MenuChoice != EXIT);
	
	FreeDynamicMemory(Character);

	return 0;
}			  
