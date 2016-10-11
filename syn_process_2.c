#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "display.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

#include <stdlib.h>
#include <sys/shm.h>



int main()
{
  int PARENT = 0;
  int CHILD = 1;
  int i;
  int flag;

  int shmid = shmget(IPC_PRIVATE,sizeof(int),0600);   //definition of shared memory 
  int *shared = (int*)shmat(shmid,0,0);     //shared memory segment attachment


  struct sembuf up = { 0,1,0 };     
  struct sembuf down = { 0,-1,0 };

  int semid = semget (IPC_PRIVATE,1,0600);    //semaphore creation
  
  semop(semid,&up,1);     //semaphore initialization to 1 

  if (fork())
  {
    for (i=0;i<10;i++)
    { 
      semop(semid,&down,1);
        display("ab"); 
        *shared = CHILD;
      semop(semid,&up,1); 
      do
       usleep(1);
      while (*shared!= PARENT);   
    }
    wait(NULL);
    
  }
  else
  {
    for (i=0;i<10;i++)
    {     
      semop(semid,&down,1);
      display("cd\n");
      *shared= PARENT;
      semop(semid,&up,1);
      do
        usleep(1);
      while (*shared != CHILD && i!=9);     
    }
  }
  shmdt((void*)shared);   //shared memory segment datachment
  shmctl(shmid,IPC_RMID,0);       //shared memory segment destruction
  flag = semctl(semid,0,IPC_RMID);    //semaphore destruction

  return 0;
}
