#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define err_sys(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }
#define MAXLINE 512

struct args {
  char **args;
  int argc;
};


#define PIPE_DELIMITER "-->"

struct args *parse_pipe(char *line) {
  char **argv = malloc(MAXLINE*sizeof(char*));
  int argc = 1;
  argv[argc-1] = line;

  int delimiter_len = strlen(PIPE_DELIMITER);
  char *new_cmd;
  while ((new_cmd = strstr(argv[argc-1], PIPE_DELIMITER)) != NULL && argc < MAXLINE) {
    for (int i = 0; i < delimiter_len; i++) {
      new_cmd[i] = 0;
    }
    new_cmd[0] = '\n';
    argc++;
    argv[argc-1] = new_cmd+delimiter_len;
  }

  argv[argc] = NULL; // null terminated seznam

  struct args *retval = malloc(sizeof (struct args));
  retval->args = argv;
  retval->argc = argc;

  return retval;
}

void exec_line(char **argv) {
  pid_t forkPid;
  int status;
  forkPid = fork();
  if (forkPid < 0) err_sys("fork err")
  if(forkPid == 0){
    if (execvp(argv[0], argv)) err_sys("execvp err");
  } else {
    if (wait(&status) < 0) err_sys("wait err")
    if (status != 0) printf("Child exit status: %d\n", status);
  }
}

void exec_piped(char **argv1, char **argv2) {
  pid_t forkPid;
  int status;
  forkPid = fork();

  if (forkPid < 0)
    err_sys("fork err")
  if(forkPid == 0){

    // 1. child
    int fd[2];
    if(pipe(fd) < 0) err_sys("pipe err")

    forkPid = fork();
    if (forkPid < 0) err_sys("fork err")
    if(forkPid == 0){
      // 2. child
      close(fd[1]); dup2(fd[0], STDIN_FILENO); close(fd[0]);
      if (execvp(argv2[0], argv2)) err_sys("execvp err");
    } else {
      // 1. child
      close(fd[0]); dup2(fd[1], STDOUT_FILENO); close(fd[1]);
      if (execvp(argv1[0], argv1)) err_sys("execvp err");
    }
  } else {
    // parent
    if (wait(&status) < 0) err_sys("wait err")
    if (status != 0) printf("Child exit status: %d\n", status);
  }
}


int sigint_counter = 0;

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


void sig_handler(int signo)
{
    if (sigint_counter)
    {
        printf("\nNasvidenje!\n");
        exit(0);
    }
    printf(">\n For exit press CTRL+C again in 3 sec.\n");
    sigint_counter++;
    alarm(3);
}

void alarm_handler(int signo){
    sigint_counter = 0;
    printf("> ");
    fflush(stdout);
}

int main()
{
    signal(SIGALRM, alarm_handler);
    signal(SIGINT, sig_handler);
	char buf[MAXLINE];
	pid_t pid;
	int status;
	printf("> "); // izpiše prompt %
    struct args *argument_list;
    struct args *argument_pipe;
	while (fgets(buf, MAXLINE, stdin) != NULL)
	{
        buf[strlen(buf) - 1] = 0; // spremeni znak za novo vrstico v ničti znak
        argument_pipe = parse_pipe(buf);
        if (argument_pipe->argc == 1)
        {
            argument_list = parse_arguments(buf);
            if(argument_list->argc > 0){
                exec_line(argument_list->args);
            }
            /*if ( (pid = fork()) < 0)
                err_sys("fork error")
            else if (pid == 0) { // otrok
                char **commands = argument_list->args;
                int code = execvp(commands[0], commands);

                printf("couldn't execute: %s", buf);
                free(argument_list);
                exit(127);
            }*/
        }else if(argument_pipe->argc == 2){
            struct args *argument_list1 = parse_arguments(argument_pipe->args[0]);
            struct args *argument_list2 = parse_arguments(argument_pipe->args[1]);
            exec_piped(argument_list1->args, argument_list2->args);
        }
		printf("> ");
	}

	exit(0);
}
