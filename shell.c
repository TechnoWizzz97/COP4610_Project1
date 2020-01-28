#include "shell.h"
void runShell(instruction* ptr)
{
	pid_t pid, wpid;
	char * args = ptr->tokens;
	int stat, fd0, fd1, i, fd[2];
	int bg_flag = 0;
	char buf;
  	//Child
  	if((p = fork()) == 0)
	{
    		for(i = 0; args[i] != '\0'; i++)
		{
      			//Input
      			if(strcmp(args[i],"<") == 0)
			{
				if(args[i+1] == NULL)
				{
				  fprintf(stderr, "Missing name for redirect\n");
				  return 1;
       				}
				else
				{
	  				if((fd0 = open(args[i+1], O_RDONLY)) < 0)
					{
           					exit(0);
          				}
					close(STDIN_FILENO);
        			}
				dup2(fd0, 0);
				close(fd0);
      			}
      			//Output
      			if(strcmp(args[i],">") == 0)
			{
				if(args[i+1] == NULL)
				{
          				fprintf(stderr, "Missing name for redirect\n");
          				return 1;
        			}
				else
				{
          				if((fd1 = open(args[i+1], O_WRONLY | O_CREAT, 0644)) < 0)
					{
			    			perror("cant open file\n");
			    			exit(0);
	  				}
					close(STDOUT_FILENO);
				}
			dup2(fd1, STDOUT_FILENO);
      			close(fd1);
    			}
     			//Pipe
    			if(strcmp(args[i],"|") == 0)
			{
				pipe(fd);
				if((p = fork()) == 0)
				{
					close(STDOUT_FILENO);
			  		dup2(fd[1], 1);
			  		close(fd[0]);
			  		close(fd[1]);
			  		execvp(args[i-1], args);
      				}
				else
				{
					close(STDIN_FILENO);
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
    		bg_flag = 0;
    		for(i = 0; args[i] != '\0'; i++)
		{
      			//Check if this is supposed to execute in the BG
      			if(strcmp(args[i], "&") == 0)
			{
        			if (i != 0 && i ==(args_size - 1))
				{
          				bg_flag = 1;
          				//printf("BG FLAG SET\n");
          				break;
        			}
				else if (i != 0)
				{
	  				bg_flag = -1;
	  				break;
				}
      			}
    		}
    		//Foreground Execution
    		if (bg_flag==1)
		{
      			do
			{
        			wpidid = waitpid(p, &stat, WUNTRACED);
      			}while (!WIFEXITED(stat) && WIFSIGNALED(stat));
    		}
    		//Background Execution
    		else if (bg_flag == 0)
		{
      			wpidid = waitpid(-1,&stat, WNOHANG);
      			printf("Background pid: %d\n", p);
      			if (wpid > 0)
			{
        			printf("Completed Background pid: %d\n", wpid);
      			}
    		}
    		//Error Signal
    		else if (bg_flag == -1) printf("Invalid Command\n");
  	}
  return 1;
}
