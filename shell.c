#include "shell.h"
void shellFunc()
{
	int run = 1;
	char* str;
	char**argv;
	startBackgroundProc();
	while(run)
	{
		chkQueue();
		printStartStr();
		str = readUserInput();
		argv = parseUserInput();
		
		if(argv[0] == NULL)
		{
			//no arguments found
		}
		else if(errorCheck(argv))
		{
			printf("Error encountered when processing IO Redirect and/or Piping and/or Background process running");
		}
		else if(strcmp(argv[0], "exit")==0)
		{
			size_t argsArray = 0;
			while (array[argsArray] != NULL)
			{
					free(array[argsArray]);
					++argsArray;
			}
			free(array);
			printf("Detected exit statement and now exiting shell...\n");
			waitProcessQueues();
			exit(0);
		}
		else if(strcmp(argv[0], "cd") == 0)
		{
			size_t i;
			for(i = 0; i != NULL; i++);
			if(i <= 2)
			{
				if(i == 2)directoryChange(argv[1]);
				else directoryChange(getenv("HOME"));
			}
			else printf("Incorrect argument input, please try again...");
		}
		else if(executable(argv[0]) == TRUE)
		{
			int status;
			pid_t pid = fork();
			if (pid == -1)
			{
				printf("Error on fork\n");
				exit(1);
			}
			else if (pid == 0)
			{
				execv(argv[0], argv);
				printf("Trouble executing: \n");
				PrintArgVector(argv);
				exit(1);
			}
			else
			{
			}
		}			
	}
}
