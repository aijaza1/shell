// Azeem Aijaz
// Project 3
// Last Date Modified: 12/4/21

/* $begin shellmain */
#include "csapp.h"
#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 
void pidFunction();
void ppidFunction();
void cdFunction(char **argv);
void helpFunction();
void exitFunction();

// pointer to current prompt
char *prompt = "my257sh"; // default prompt

// signal handler
void sigint_handler(int sig){
	printf("\n%s> ", prompt);
	fflush(stdout);
	return;
}

int main(int argc, char *argv[]) 
{
    char cmdline[MAXLINE]; /* Command line */
   

    // if there are 3 arguements in command line
    if(argc == 3)
	prompt = argv[2]; // rename the prompt
    

    // signal handler
    signal(SIGINT, sigint_handler);

    while (1) {
    /* Read */
        printf("%s> ", prompt); // to keep on printing the prompt                  
	Fgets(cmdline, MAXLINE, stdin); 
        if (feof(stdin))
        	exit(0);
        					   	
       	/* Evaluate */
        eval(cmdline);
    } 
}
/* $end shellmain */

       					    		     
/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
	char *argv[MAXARGS]; /* Argument list execve() */
        char buf[MAXLINE];   /* Holds modified command line */
        int bg;              /* Should the job run in bg or fg? */
        pid_t pid;           /* Process id */
        					    		                          
        strcpy(buf, cmdline);
        bg = parseline(buf, argv); 
        if (argv[0] == NULL)  
        return;   /* Ignore empty lines */
	
	if (!builtin_command(argv)) { 
        	if ((pid = Fork()) == 0) {   /* Child runs user job */
      			if (execvp(argv[0], argv) < 0) {
				printf("Execution failed (in fork)\n");
        			printf("%s: Command not found.\n", argv[0]);
        			exit(1);
				
      			 }
      	         }
        					    		                                      	                                                                            
    /* Parent waits for foreground job to terminate */
    if (!bg) {
       	int status;
        if (waitpid(pid, &status, 0) < 0)
            unix_error("waitfg: waitpid error");

	// if the process exits correctly
	if(WIFEXITED(status)){
		int exitSt = WEXITSTATUS(status);
		printf("Process exited with status code %d\n", exitSt); // print the exit status code if successful
	}
   }
   else
    	printf("%d %s", pid, cmdline);
	}
   
   return;
   }
   
  /* If first arg is a builtin command, run it and return true */
  int builtin_command(char **argv) 
  {
	// if command is exit
	if(!strcmp(argv[0], "exit")){
		exitFunction();			
		exit(0);
	}
	// if command is pid
	else if(!strcmp(argv[0], "pid")){
		pidFunction();
		return 1;
	}
	// if command is ppid
	else if(!strcmp(argv[0], "ppid")){
		ppidFunction();
		return 1;
	}
	// if command is help
	else if(!strcmp(argv[0], "help")){
		helpFunction();
		return 1;
	}
	// if command is cd
	else if(!strcmp(argv[0], "cd")){
		cdFunction(argv);
		return 1;
	}
	return 0;	
        					    		                                      }
  /* $end eval */

        					    		                                      /* $begin parseline */
  /* parseline - Parse the command line and build the argv array */
  int parseline(char *buf, char **argv) 
  {
      char *delim;         /* Points to first space delimiter */
      int argc;            /* Number of args */
      int bg;              /* Background job? */
      
      buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
      while (*buf && (*buf == ' ')) /* Ignore leading spaces */
      	 buf++;
      
      /* Build the argv list */
      argc = 0;
      while ((delim = strchr(buf, ' '))) {
          argv[argc++] = buf;
          *delim = '\0';
          buf = delim + 1;
          while (*buf && (*buf == ' ')) /* Ignore spaces */
              buf++;
      }
      argv[argc] = NULL;
 
      if (argc == 0)  /* Ignore blank line */
	  return 1;					    		                                      	                                                                            			    	    					                	    	                        	            				                            	
      /* Should the job run in the background? */
      if ((bg = (*argv[argc-1] == '&')) != 0)
     	  argv[--argc] = NULL;
        					    		                                     	  return bg;
        					    		                                    }
        					    		                                    /* $end parseline */
void pidFunction(){
	printf("%d\n", getpid());
	return;
}  
void ppidFunction(){
	printf("%d\n", getppid());
	return;
}
void cdFunction(char **argv){
	if(argv[1] == NULL){ // if just 'cd' is wrote, print the current directory
		char dir[100];
		getcwd(dir, sizeof(dir)); // get the current working directoy and put into cwd
		printf("Current directory: %s\n", dir);
	}
	else if(argv[1] != NULL){ // if the argument after cd is not empty, change the directory to that
		chdir(argv[1]);
	}

	return;	
}
void helpFunction(){
	printf("********************************************\n");
	printf("A Custom shell for CMSC 257\n");
	printf("   - Developer Name: Azeem Aijaz\n");
	printf("Usage: \n");
	printf("   - To change shell prompt, ./sh257 -p <prompt>\n");
	printf("********************************************\n\n");
	printf("BUILTIN COMMANDS: \n");
	printf("   - pid(print process id of the shell)\n");
	printf("   - ppid(print parent process id of the shell)\n");
	printf("   - cd(print current working directory or change current working directory)\n");
	printf("   - exit(exit the shell)\n"); 
	printf("   - help(prints useful information to use the shell) \n");
	printf("SYSTEM COMMANDS: \n");
	printf("   - You can use man pages to get help for certain non-built-in commands.\n");
	
	return;
}
void exitFunction(){
	raise(SIGTERM);
	return;
}
   					    		                                      	                                                                            			    	    					                	    	                        	            				                            	        	    
