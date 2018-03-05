/*
 * Project 1 - Shell
 * Programmer: Jordan Long
 * Instructor: S. Lee
 * Course: SMPE 320
 * Section 1
 */


#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

/* The maximum length command */
#define MAX_LINE 80




/*
 * Takes the command and arguments and executes it.
 */
void execute(char **arguments)
{
	if(execvp(arguments[0], arguments) < 0)
	{
		perror("Could not execute command.");
	}
}


/*
 * Takes in the command and arguments and forks the parent process.
 */
void createForkedProcess(char **arguments)
{

	//Fork child process from parent
	pid_t pid;
	pid = fork();
	//If pid goes negative, fork has failed. Throws error and exits.
	if (pid < 0)
	{
		perror("Error: Fork failed.");
		exit(-1);
	}

	// pid is 0? a new process was created, and this copy is it
	if (pid == 0)
	{
		execute(arguments);
		// this should never return - so if it doesn't, something bad happened.
		abort( );
	}

	// pid is not 0? then it is the pid of the child
	else
	{
		int status;
		printf("Waiting for command to finish.\n");
		waitpid(pid, &status, 0);
	}
}

/**
 * Counts the number of arguments in the string.
 * Returns an integer value of number of arguments in string.
 */
int countArguments(const char *args)
{
	int count=0, len;
	char lastC;
	len = strlen(args);
	if(len>0)
	{
		lastC = args[0];
	}
	for(int i=0; i<=len; i++)
	{
		if((args[i]==' '|| args[i]=='\0') && lastC !=' ')
		{
			count++;
		}
		lastC = args[i];
	}

	return count;
}



/**
 * given a command line string, call on countArguments to count the # of arguments present,
 * mallocate an array of pointers to hold that # of string pointers, call on
 * nextArgument that number of times, storing the resulting pointer to a string into each element of the array.
 */

void buildCommandLine(char *args)
{

	int numOfArgs = countArguments(args);
	char *parsedArguments[numOfArgs+1];
	char *token;
	int i = 0;

	token =strtok(args, " ");
	while(token != NULL)
	{
		parsedArguments[i] = token;
		i++;
		token = strtok(NULL, " ");
	}
	parsedArguments[numOfArgs+1] = NULL;
	createForkedProcess(parsedArguments);
}

/*
 * Main function for the Shell program.
 */
int main(void)
{
	char args[MAX_LINE/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */
	int background = 0;

	while (should_run != 0)
	{
		printf("osh>");
		fflush(stdout);
		fgets(args, MAX_LINE/2+1, stdin);

//		printf("User Input: %s\n", args);
		buildCommandLine(args);



		/*
		 * If no text or too much text was inputed, throw an error.
		 */
//		if(strchr(&args[0] == "\0"))
//		{
//			perror("An error occurred");
//		}

		/*
		 * If user types in exit, set should_run to 0 and terminate.
		 */
		if(strcmp(&args[0], "exit") == 0)
		{
			should_run = 0;
			return 0;
		}

	}

	return 0;
}


