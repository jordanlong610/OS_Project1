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

///*
// * Define history linked list
// */
//struct node{
//	int key;
//	char *data;
//	struct node* next;
//};
//
//struct node *head = NULL;
//struct node *current = NULL;

int historyCounter = 0;



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
void createForkedProcess(char **arguments, int wait)
{
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

	/*
	 * If pid is not 0? then it is the pid of the child.
	 * If wait flag is 1, parent will wait for child to finish before exiting.
	 */
	else
	{
		int status;
		if(wait == 1)
		{
			waitpid(pid, &status, 0);
		}
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
	char *parsedArguments[numOfArgs];
	char *token;
	int wait, i = 0;

	token = strtok(args, " ");
	while(i<numOfArgs)
	{
		parsedArguments[i] = token;
		i++;
		token = strtok(NULL, " ");
	}

	/*
	 * If & included in command, increment wait value to 1, so program knows to not wait.
	 * Default: 0, Wait: 1
	 */

	if(*parsedArguments[i-1] == '&')
	{
		wait = 1;
		parsedArguments[i-1] = NULL;
	}
	parsedArguments[numOfArgs] = NULL;
	createForkedProcess(parsedArguments, wait);
}

/**
 * Structure of the node used in the history linked list.
 */
struct node{
	int id;
	char *command;
	struct node *next;
};

/*
 * Initializes the linked list for history by creating a head node.
 */
void init(struct node **head)
{
	*head = malloc(sizeof(struct node));
	(*head)->id = 0;
	(*head)->command = NULL;
	(*head)->next = NULL;
}

/*
 * Inserts a command used to the front of the linked list.
 * Then points the head at that node.
 */
void insert(struct node **head, int id, char *command)
{
	struct node *new = malloc(sizeof(struct node));

	new->next = NULL;
	new->id = id;
	new->command = malloc(strlen(new)+1);
	strcpy(new->command, command);

	new->next = *head;
	*head = new;
}

/*
 * Find a command with its id. Return the node containing the id and command
 */
struct node* find(struct node **head, int id)
{
   struct node* current = *head;

   //if list is empty
   if(head == NULL)
   {
      return NULL;
   }

   while(current->id != id)
   {
      if(current->next == NULL)
      {
         return NULL;
      }
      else
      {
         current = current->next;
      }
   }
   return current;
}

/*
 * Prints the 10 most recent commands to the console.
 */
void print(struct node *head)
{
	int count = 0;
	if ((head->next == NULL) && (count<10))
	{
		return;
	}
	else
	{
		printf("%d %s\n", head->id, head->command);
		count++;
		print(head->next);
	}
}











/*
 * Main function for the Shell program.
 */
int main(void)
{
	char args[MAX_LINE/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */

	/*
	 * Create and initialize the linked list for history.
	 */
	struct node *head;
	init(&head);

	/*
	 * While user has not typed exit the console will continue to run.
	 */
	while (should_run != 0)
	{
		printf("osh>");
		fflush(stdout);
		fgets(args, MAX_LINE/2+1, stdin);

		//Remove the newline character
		char *token = strtok_r(args, "\n", &token);
		printf("User Input: %s\n", token);

		/*
		 * If user types in exit, set should_run to 0 and terminate.
		 */
		if(strcmp(&args[0], "exit") == 0)
		{
			should_run = 0;
			printf("Closing Shell.");
		}

		/*
		 * If history was entered, display history of commands.
		 */
		else if(strcmp(&args[0], "history") == 0)
		{
			print(head);
		}

		/*
		 * If !! was entered, run the last successful command.
		 */
		else if(strcmp(token, "!!") == 0)
		{
			struct node *temp = malloc(sizeof(struct node));
			temp = find(&head, historyCounter-1);
			printf("Running last successful command.\n");
			printf("ID from pointer %d\n", temp->id);
			printf("Command from pointer %s\n", temp->command);
			buildCommandLine(temp->command);
		}



		/*
		 * If !# was entered, run that specific command.
		 */
		else if(args[0] == '!' && isdigit(args[1]))
		{
			/*
			 * Remove the ! from the rest of the number.
			 */
			char *tok = strtok(token, "!");
			tok = strtok(tok,"\n");
//			printf("number typed: %s", tok);

			struct node *temp = malloc(sizeof(struct node));
			temp = find(&head, atoi(tok));
			printf("ID from pointer %d\n", temp->id);
			printf("Command from pointer %s\n", temp->command);
			buildCommandLine(temp->command);


		}















		/*
		 * If no text or too much text was inputed, throw an error.
		 */
//		else if(strcmp(&args[0] == "\0") == 0)
//		{
//			perror("An error occurred");
//		}

		/*
		 * If its a good command entered, build and run it.
		 */
		else
		{
			//Store parsed command as node in linked list
			insert(&head, historyCounter, token);
			historyCounter++;
			buildCommandLine(token);
		}
	}

	return 0;
}


