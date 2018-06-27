#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAX_THREADS 50

pthread_t thread_id[MAX_THREADS]; 

void *process()
{

  printf("thread created\n");
  return NULL;
}


int main(int argc, char *argv[])
{

  int i, n;

  if(argc < 3){
      printf("Please add the number of threads to the command line\n");
      exit(1); 
  }

  if(!strcmp(argv[1], "-n")){
    n = atoi(argv[2]);
    if(n > MAX_THREADS) n = MAX_THREADS;

  }
  else printf("Wrong args\n");

  for(i = 0; i < n; i++)
  {
    if(pthread_create(&thread_id[i], NULL, process, NULL)) {
      fprintf(stderr, "Error creating thread\n");
      return 1;
    }

    if(pthread_join(thread_id[i], NULL)) {
      fprintf(stderr, "Error joining thread\n");
      return 2;
    }
  }

  pthread_exit(NULL);
  return 0;
}