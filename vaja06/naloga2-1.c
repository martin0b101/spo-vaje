#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>


#define err(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};



void lock_semaphore(int sem_id){
	struct sembuf operations[1];
	int sem_lock_num;
	operations[0].sem_num = 0; // kateri semafor
  	operations[0].sem_op = -1; // katera operacija
  	operations[0].sem_flg = 0; // zastavica (čakanje)
  	if (semop(sem_id, operations, 1) < 0 ) 
		err("semop lock")
}

void unlock_semaphore(int sem_id){
	struct sembuf operations[1];
	int sem_lock_num;
	operations[0].sem_num = 0; // kateri semafor
	operations[0].sem_op = 1; // katera operacija
	operations[0].sem_flg = 0; // zastavica (čakanje)
  	if (semop(sem_id, operations, 1) < 0 ) 
		err("semop unlock")
}

void main()
{
	// priklop na semafor
	key_t sem_key = ftok(".", 'b');
	if (sem_key < 0)
		err("ftok")
	int sem_id = semget( sem_key, 1, 0664 );
	if (sem_id < 0)
		err("semget (semaforja ni)")
	
	struct sembuf operations[1];

	int shmid;
	char *addr;
	key_t shm_key = ftok(".", 'A');
	if ( (shmid = shmget(shm_key, 0, 0644 | IPC_CREAT)) < 0)
		err("shmid")

	lock_semaphore(sem_id);

	if ( (addr = (char *)shmat( shmid, (void *)0, 0)) < 0)
		err("shmat")

	unlock_semaphore(sem_id);
	puts(addr);
	
	if ( shmdt(addr) < 0)
		err("shmdt")

	//delete semaphore
	if(semctl( sem_id, 0, IPC_RMID, NULL ) < 0) 
		err("semctl delete")

	
}