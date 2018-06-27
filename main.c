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
  int *available;

  if(argc < 4){
      printf("Please add the number of threads to the command line\n");
      exit(1); 
  }

  if(!strcmp(argv[1], "-n")){
    n = atoi(argv[2]);
    if(n > MAX_THREADS) n = MAX_THREADS;

  }
  else{

    printf("Wrong args\n");
    exit(1);
  }

  if(!strcmp(argv[3], "-a")){
    available = malloc((argc-3)*sizeof(int));
    for(i=0;i<(argc-4);i++){
      available[i] = atoi(argv[i+4]);
    }
  }
  else{

    printf("Wrong args\n");
    exit(1);
  }


  for(i=0;i<n;i++)
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
  free(available);
  return 0;
}