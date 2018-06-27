#include <pthread.h>
#include <stdio.h>


void *process()
{

  printf("thread created\n");
  return NULL;
}


int main(int argc, char *argv[])
{

  pthread_t thread;

  if(pthread_create(&thread, NULL, process, NULL)) {
    fprintf(stderr, "Error creating thread\n");
    return 1;
  }

  if(pthread_join(thread, NULL)) {
    fprintf(stderr, "Error joining thread\n");
    return 2;
  }

  return 0;
}