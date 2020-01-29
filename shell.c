#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shell.h"

void runShell(instruction* input)
{
	pid_t pid, wpid;
	char** args = input->tokens;
	int stat, fd0, fd1, i, fd[2];
	int bg_flag = 1;

	int p = fork();
	if(p  == 0)
	{
    for(i = 0; args[i] != NULL; i++)
		{
			if(strcmp(args[i], "exit") == 0)
			{
				exit(0);
			}
			//Input
      if(strcmp(args[i], "<") == 0)
			{
				if(args[i+1] == NULL)
				{
				  fprintf(stderr, "Missing name for redirect\n");
				  return;
       	}
				else
				{
	  			if((fd0 = open(args[i+1], O_RDONLY)) < 0)
					{
           	exit(0);
          }
					close(0);
        }
				dup2(fd0, 0);
				close(fd0);
      }
  		//Output
      if(strcmp(args[i], ">") == 0)
			{
				if(args[i+1] == NULL)
				{
          fprintf(stderr, "Missing name for redirect\n");
        	return;
        }
				else
				{
          if((fd1 = open(args[i+1], O_WRONLY | O_CREAT, 0644)) < 0)
					{
			    	perror("cant open file\n");
			    	exit(0);
	  			}
					close(1);
				}
				dup2(fd1, STDOUT_FILENO);
      	close(fd1);
    	}
     	//Pipe
    	if(strcmp(args[i], "|") == 0)
			{
				pipe(fd);
				if((p = fork()) == 0)
				{
					close(1);
			  	dup2(fd[1], 1);
			  	close(fd[0]);
			  	close(fd[1]);
			  	execvp(args[i-1], args);
      	}
				else
				{
					close(0);
			  	dup2(fd[0], 0);
			  	close(fd[0]);
			  	close(fd[1]);
			  	execvp(args[i+1], args);
				}
			}
      if(strcmp(args[i], "&") == 0)
			{
				if(i != 0 && i == sizeof(args-1))
				{
      	  bg_flag = 1;
	  			break;
				}
      }
    }
    if(execvp(args[0], args) == -1)
		{
      perror("shell");
    }
    exit(EXIT_FAILURE);
  }
  //Error
  else if(p < 0)
	{
		perror("shell");
  }
  //Parent
  else
	{
    bg_flag = 1;
    for(i = 0; args[i] != NULL; i++)
		{
    	//Check if this is supposed to execute in the BG
    	if(strcmp(args[i], "&") == 0)
			{
        if (i != 0 && i == strlen(*args)-1)
				{
          bg_flag = 0;
					input->tokens[i] = " ";
        }
				else if (i <= 0)
				{
	  			bg_flag = -1;
				}
      }
    }
    //Foreground Execution
    if(bg_flag==1)
		{
      do
			{
        wpid = waitpid(p, &stat, WUNTRACED);
      } while (!WIFEXITED(stat) && WIFSIGNALED(stat));
    }
    //Background Execution
    else if (bg_flag == 0)
		{
      wpid = waitpid(-1,&stat, WNOHANG);
      printf("Background pid: %d\n", p);
      if (wpid > 0)
			{
        printf("Completed Background pid: %d\n", wpid);
      }
    }
  	//Error Signal
		else if(bg_flag == -1) printf("Invalid Command\n");
  }
	while(wpid <= 0) wpid = waitpid(-1,&stat, WNOHANG);
  return;
}

/*char** removeSpecialChar(char** tokes)
{
	for(int j = 0; )
}*/
