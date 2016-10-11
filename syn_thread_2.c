#include <stdio.h>
#include "display.h"
#include <pthread.h>

#define FALSE 0
#define TRUE 1

pthread_mutex_t locker;   //mutex declaration
pthread_cond_t condition;   //condition variable declaration
int predicate = FALSE;


void* message1(void* data)
{
  int i;  
  for (i = 0 ; i <10 ;i++)
  {  
   
    pthread_mutex_lock(&locker); 
     //wait for turn
    while(predicate != FALSE)
      pthread_cond_wait(&condition,&locker);
   

    display(data);

    //set predicate to TRUE and wake up thread 2
    
    predicate = TRUE;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&locker);
  }
  pthread_exit(NULL);
  
}

void* message2(void* data)
{
  int j;  
  for (j = 0 ; j <10 ;j++)
  {  
    
    pthread_mutex_lock(&locker); 
    //wait for turn 
    while(predicate != TRUE)
      pthread_cond_wait(&condition,&locker);
    
   

    display(data);

    //set predicate to FALSE and wake up thread 1
    
    predicate = FALSE;
    pthread_cond_signal(&condition);
    pthread_mutex_unlock(&locker);
  }
  pthread_exit(NULL);
  
}

int main()
{
  int temp = pthread_mutex_init(&locker, NULL);
  pthread_t t1,t2;

  pthread_create(&t1,NULL,message1,"ab");
  pthread_create(&t2,NULL,message2,"cd\n");  
  
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  pthread_mutex_destroy(&locker);

  return 0;

}
