#include <stdio.h>
#include <string.h>

#define HASHTABLESIZE 10

int MyHashFunction(char *name) 
{
    int value = 0;

    for (int i = 0; i < strlen(name); i++) 
    {
        value += name[i];
    }

    return value % HASHTABLESIZE;
}

int main(void)
{
    char searchName[20];
    printf("Enter a character name: ");
    fgets(searchName, sizeof(searchName), stdin);

    if (searchName[strlen(searchName) - 1] == '\n')
    {
        searchName[strlen(searchName) - 1] = '\0';
    }

    printf("The hash value for %s is %d\n", searchName, MyHashFunction(searchName));

    return 0;
}