#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define err(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }

#define MAX_SIZE 16000


struct my_msgbuf {
	long mtype;
	char mtext[MAX_SIZE];
};

void count_words(char words[]){
    int number_of_words = 0;
    for (int i = 0; words[i] != 0; i++)
    {
        if (words[i] == ' ' && words[i+1] != ' ')
        {
            number_of_words++;
        }
    }
    printf("Number of words are %d.\n", number_of_words+1);
}

void main()
{
	key_t key; // = 1234;
	int id;
	struct my_msgbuf msgbuf;
	
	if ( (key = ftok("/usr/lib", 'q')) < 0)
		err("ftok")

	if ( (id = msgget( key, 0666 | IPC_CREAT)) < 0)
		err("msgget")
		
	//if ( msgrcv( id, &msgbuf, sizeof(msgbuf.mtext), 5, 0) < 0)
	//if ( msgrcv( id, &msgbuf, sizeof(msgbuf.mtext), 9, IPC_NOWAIT) < 0)
	if ( msgrcv( id, &msgbuf, sizeof(msgbuf.mtext), 0, IPC_NOWAIT) < 0)	// vzame prvo sporočilo
		printf("Ni ustreznega sporocila!\n");
	else
		count_words(msgbuf.mtext);
	//puts(msgbuf.mtext);
    //printf("%s", msgbuf.mtext);
	
	if ( msgctl( id, IPC_RMID, NULL) < 0)
		err("msgctl")
}