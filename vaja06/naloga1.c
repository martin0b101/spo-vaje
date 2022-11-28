#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define err(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }

#define MAX_SIZE 16000

struct my_msgbuf {
	long mtype;
	char mtext[MAX_SIZE];
};



void main(int argc, char *argv[])
{
	
    if (argc < 2)
    {
        err("Add file");
    }
    
    key_t key; // = 1234;
	int id;
	struct my_msgbuf msgbuf;
	
	if ( (key = ftok("/usr/lib", 'q')) < 0)
		err("ftok")
	
	if ( (id = msgget( key, 0666 | IPC_CREAT)) < 0)
		err("msgget")
	
	printf("Key: %d, id = %d\n", key, id);

    
    int file = open(argv[1], O_RDONLY);
    char buff[MAX_SIZE];
    int n = read(file, buff, MAX_SIZE);
    
    for (int i = 0; i < strlen(buff); i++)
    {
        if(buff[i] == '\n'){
            buff[i] = ' ';
        }
    }
    
    printf("\n");

	msgbuf.mtype = 16000;

    //printf("inside buff %s", buff);
	//strcpy( msgbuf.mtext, "To je sporocilo.");
	strcpy(msgbuf.mtext, buff);
    //msgbuf.mtext = buff;
    //printf("inside struct: %s", msgbuf.mtext);

	if ( msgsnd( id, &msgbuf, strlen(msgbuf.mtext)+1, 0)< 0)
		err("msgsnd")
	
}