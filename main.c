#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>

#define BUFF_SIZE 10 //size of buffer

//indecies of semaphors
#define MUTEX 0
#define EMPTY 1
#define FULL 2


//function prototypes
void *producer(void*);void *consumer(void*);
void up(int, int);
void down(int, int);
void consume(char*);
char produce();
int getVal(int, int);


//static buffer for producer and consumer to read from

//explicit declaration of semaphore
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
 };



char buffer[BUFF_SIZE];;

int main()
{
key_t keyz;//store key for semaphore access
int semid;//get id for semahors
pthread_t threads[2];//threads
union semun un[3];
un[MUTEX].val = 1;
un[FULL].val = 0;
un[EMPTY].val = BUFF_SIZE;
srand(time(NULL));



if ((keyz = ftok("task2.c", 'F')) == -1)//access keys based and id based on this file
	perror("error getting the key");


if((semid = semget(keyz, 3, 0666 | IPC_CREAT ) ) == -1) //create three mutexs
	perror("error creating sems");



if(semctl(semid, MUTEX, SETVAL, un[MUTEX]) == -1 || //intialize mutex to 1 to be used
   semctl(semid, FULL, SETVAL, un[FULL]) == -1 || //intialize full to 0 until items inserted
   semctl(semid, EMPTY, SETVAL, un[EMPTY]) == -1 )//intialize empty to bufer size
	perror("error intializing sems");


//start threads and join
if(pthread_create(&threads[0], NULL, producer, (void*)(intptr_t) semid) != 0 || 
   pthread_create(&threads[1], NULL, consumer, (void *)(intptr_t)  semid) !=0 )
	perror("Error creating thread");

pthread_join(threads[0], NULL);
pthread_join(threads[1], NULL);


exit(0);


}



void *producer(void * smid)
{
	int semid = (intptr_t) smid;
	int item, index;

	while(1)
	{

	item = produce();
	down(EMPTY, semid);
	down(MUTEX, semid);
	index = getVal(FULL, semid);
	up(MUTEX, semid);
	up(FULL, semid);
	buffer[index] = item;
	}
}


void *consumer(void *smid)
{
	int semid = (intptr_t) smid;
	int total = 0;
	int item, index;

	while(1)
	{


		index = getVal(FULL, semid);
		if(index > 0)
		{
			down(FULL, semid);
			down(MUTEX, semid);
  			consume(&buffer[index-1]);
			up(MUTEX, semid);
			up(EMPTY, semid);
			printf("%s\n",buffer);
		}
	}


}


char produce(){ return rand() % (126 - 33 + 1) + 33; }


void consume(char* item){item = '\0';} 


int getVal(int target, int semid)
{

int n;
if((n = semctl(semid, target, GETVAL)) == -1)
	perror("Dude super");

return n;

}




void up(int target, int semid)
{

struct sembuf s = {target , 1 , 0};

if(semop(semid, &s, 1) == -1)
	perror("error performing UP");


}


void down(int target, int semid)
{

struct sembuf s = {target , -1 , 0};

if(semop(semid, &s, 1) == -1)
        perror("error performing DOWN");


}
