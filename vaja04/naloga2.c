#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define err_sys(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }
#define MAXLINE 512

struct args {
  char **args;
  int argc;
};

struct args* parse_arguments(char* line){
    char** args;
    args = malloc(MAXLINE*sizeof(char*));
    int counter = 0;
    int new_arg_started = 0;
    int in_len = strlen(line);
    for (int i = 0; i < in_len && i < MAXLINE-1; i++) {
        if (line[i] == '\n') {
            line[i] = 0;
            break;
        } else if (line[i] == ' ') {
            if (!new_arg_started) { /* Do nothing (dupliciran space) */ }
            new_arg_started = 0;
            line[i] = 0;
        } else {
            // vsebina!
            if (!new_arg_started) {
                counter++; // nova vsebina
                args[counter-1] = &line[i];
            }
            new_arg_started = 1;
        }
    }
    args[counter+1] = NULL;
    //printf("im outside of while lop\n");
    struct args *arguments = malloc(sizeof(struct args));
    arguments->args = args;
    arguments->argc = counter;

    return arguments;
}

int main()
{
	char buf[MAXLINE];
	pid_t pid;
	int status;
	printf("> "); // izpiše prompt %
    struct args *argument_list;
	while (fgets(buf, MAXLINE, stdin) != NULL)
	{
        buf[strlen(buf) - 1] = 0; // spremeni znak za novo vrstico v ničti znak
        argument_list = parse_arguments(buf);
        if ( (pid = fork()) < 0)
			err_sys("fork error")
		else if (pid == 0) { // otrok
            char **commands = argument_list->args;
            int code = execvp(commands[0], commands);

			printf("couldn't execute: %s", buf);
            free(argument_list);
			exit(127);
		}
		// starš
		if ( (pid = waitpid(pid, &status, 0)) < 0)
			err_sys("waitpid error")
		printf("> ");
	}

	exit(0);
}
