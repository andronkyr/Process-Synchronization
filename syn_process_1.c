#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include "display.h"

int main()
{
  int i;

  struct sembuf up = { 0,1,0};
  struct sembuf down = { 0,-1,0};

  int semid = semget (IPC_PRIVATE,1,0600);
  int flag;
  semop(semid,&up,1);

  if (fork())
  {     
    for (i=0;i<10;i++)
    {
      semop(semid,&down,1); 
      display("Hello world\n");
      semop(semid,&up,1);
    }     
    wait(NULL);      
  }
  else
  {         
    for (i=0;i<10;i++) 
    {
      semop(semid,&down,1);
      display("Kalimera kosme\n");
      semop(semid,&up,1);
    }    
  }

  flag = semctl(semid,0,IPC_RMID);
  return 0;
}
