#include <stdio.h>
#include "display.h"
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t locker;     //mutex declaration

void* printer(void* data)
{
  int i;  
  for (i = 0 ; i <10;i++)
  {  
    pthread_mutex_lock(&locker);  
    display(data);    
    pthread_mutex_unlock(&locker);
  }
  pthread_exit(NULL);

}




int main()
{
  int temp = pthread_mutex_init(&locker, NULL);   //mutex initialization
  int i;
  pthread_t t1,t2;  
  
  pthread_create(&t1,NULL,printer,"Hello world \n");
  pthread_create(&t2,NULL,printer,"Kalimera kosme \n");
  
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  pthread_mutex_destroy(&locker);     //mutex destruction
  return 0;
}


