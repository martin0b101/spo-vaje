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
	// ustvari semafor
	key_t sem_key = ftok(".", 'b');
	if (sem_key < 0) 
		err("ftok")
	int sem_id = semget( sem_key, 1, 0664 | IPC_CREAT );
	if (sem_id < 0)
		err("semget")

	// nastavi semafor
	union semun sem_arg;
	sem_arg.val = 1;
	if(semctl( sem_id, 0, SETVAL, sem_arg) < 0) 
		err("semctl set")

	int shmid;
	char *addr;
	key_t key_shm = ftok(".", 'A');
	//lock
	lock_semaphore(sem_id);
	//shm
	int pid = getpid();
	char * mypid = malloc(22);   // ex. 34567
	sprintf(mypid, "PID=%d", pid);
	printf("My pid %d\n", pid);
	if ( (shmid = shmget(key_shm, 1024, 0644 | IPC_CREAT)) < 0)
		err("shmid")
	if ( (addr = (char *)shmat( shmid, (void *)0, 0)) < 0)
		err("shmat")
	strcpy( addr, mypid);
	if ( shmdt(addr) < 0)
		err("shmdt")
    sleep(10);

	unlock_semaphore(sem_id);

}