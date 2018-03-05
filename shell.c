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





/**
 * count the number of arguments.
 */
int countArguments(const char *commandLine)
{
	int num=0;


	return num;
}

/**
 * given a pointer to a string, find the start of the next argument and the end of the argument,
 * malloc a new string of that length, and return it the new string.
 */
const char *nextArgument(const char *currentString)
{


}


/**
 * given a command line string, call on countArguments to count the # of arguments present,
 * mallocate an array of pointers to hold that # of string pointers, call on
 * nextArgument that number of times, storing the resulting pointer to a string into each element of the array.
 */

char **buildCommandLine(const char *commandLine)
{




}



/*
 * Takes in the command and arguments and forks the parent process.
 */
void createForkedProcess(char *arguments)
{

	//Fork child process from parent
	pid_t pid = fork( );
	//If pid goes negative, fork has failed. Throws error and exits.
	if (pid < 0)
	{
		perror("Error: fork failed.");
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
		printf("This is still the parent: child exited with status %x\n", status);
	}
}

/*
 * Takes the command and arguments and executes it.
 */
void execute(const char *arguments)
{
	if(execvp(arguments[0], arguments) < 0)
	{
		perror("Could not execute command.");
	}
}


/*
 * Main function for the Shell program.
 */
int main(void)
{
	char *args[MAX_LINE/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */

	while (should_run)
	{
		printf("osh>");
		fflush(stdout);
		fgets(args, MAX_LINE/2+1, stdin);

		/*
		 * If no text or too much text was inputed, throw an error.
		 */
		if(args[0] == NULL)
		{
			perror("An error occurred");
		}

		/*
		 * If user types in exit, set should_run to 0 and terminate.
		 */
		else if(strcasecmp(args, "exit") == 0)
		{
			should_run = 0;
			return 0;
		}

		//Create fork with command and arguments
		createForkedProcess(args);





	}



	return 0;
}


