// The MIT License (MIT)
// 
// Copyright (c) 2024 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>

#define WHITESPACE " \t\n"
#define MAX_COMMAND_SIZE 255
#define MAX_NUM_ARGUMENTS 32

void get_command( char *command_string )
{
  char error_message[30] = "An error has occurred\n";

  char *token[MAX_NUM_ARGUMENTS];

  int token_count = 0;                                 
                                                           
  char *argument_pointer;                                         
                                                           
  char *working_string  = strdup( command_string );                
    
  char *head_ptr = working_string;
    
  while ( ( (argument_pointer = strsep(&working_string, WHITESPACE ) ) != NULL) &&
            (token_count<MAX_NUM_ARGUMENTS))
  {
    token[token_count] = strndup( argument_pointer, MAX_COMMAND_SIZE );
    if( strlen( token[token_count] ) == 0 )
    {
      token[token_count] = NULL;
    }
    else
    {
      token_count++;
    }
  }

  if(token[0]==NULL)
  {
    return;
  }

  if( strcmp(token[0], "exit") == 0 )
  {
    if ( token[1] != NULL )
    {
      write(STDERR_FILENO, error_message, strlen(error_message));
    }

    exit(0);
  }

  else if( strcmp(token[0], "cd") == 0 )
  {
    if ( chdir( token[1] ) != 0 )
    {
      write(STDERR_FILENO, error_message, strlen(error_message));
    }
  }

  else
  {
    pid_t pid = fork();

    if ( pid == -1 )
    {
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(1);
    }

    else if( pid == 0 )
    {
      int token_index;
      for( token_index = 1; token_index < token_count; token_index++ )
      {
        if( token[token_index] == NULL ) break;
        if( strcmp( token[token_index], ">" ) == 0 )
        {
          int fd = open( token[token_index+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR );
          if( fd < 0 )
          {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(0);                    
          }
          dup2( fd, 1 );
          close( fd );
            
          // Trim off the > output part of the command
          token[token_index] = NULL;

          if ( token[token_index+2] != NULL )
          {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(0);   
          }
        }
      }

      int ret = execvp( token[0], &token[0] );

      if( ret == -1 )
      {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(0);
      }
    }
    else
    {
      int status;
      waitpid( pid, &status, 0 );
    }
  }

  free( head_ptr );

  int token_index  = 0;
  for( token_index = 0; token_index < token_count; token_index ++ ) 
  {
    free(token[token_index]);
    token[token_index] = NULL;
  }

}

int main( int argc, char * argv[] )
{
  char * command_string = (char*) malloc( MAX_COMMAND_SIZE );

  char error_message[30] = "An error has occurred\n";

  FILE *fp = NULL;

  if ( argc == 2 )
  {
    fp = fopen( argv[1], "r" );

    if ( fp == NULL )
    {
      write(STDERR_FILENO, error_message, strlen(error_message));
      exit(1);
    }
  }
  else if ( argc > 2 )
  {
    write(STDERR_FILENO, error_message, strlen(error_message));
    exit(1);
  }

  while( 1 )
  {
    if ( fp == NULL )
    {
      // Batch Mode
      printf ("msh> ");
      while( !fgets (command_string, MAX_COMMAND_SIZE, stdin) );
      get_command( command_string );
    }

    else
    {
      // Interactive Mode
      while( fgets (command_string, MAX_COMMAND_SIZE, fp) != NULL)
      {
        get_command(command_string);
      }
      if( feof(fp) )
      {
        exit(0);
      }
    }
  }

  free(command_string);

  return 0;
}