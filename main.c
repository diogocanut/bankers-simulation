#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define MAX_THREADS 50

pthread_t thread_id[MAX_THREADS];
pthread_mutex_t lock;
int *total;
int *disponiveis;
int args;

void *process()
{
  long unsigned int pid = syscall(SYS_gettid);
  printf("thread created, pid: %lu\n", pid);

  int i;
  int *recursos;

  recursos = calloc((args-3), sizeof(int));

  pthread_mutex_lock(&lock);

  for(i=0;i<(args-4);i++){
    recursos[i] = (int) rand() % (total[i]+1);

  }

  pthread_mutex_unlock(&lock);


  free(recursos);
  recursos = NULL;

  return NULL;
}

int safe_state(int recursos[]){
  int i, var;
  int count = 0;
  for(i=0;i<(args-4);i++){
    var = disponiveis[i] - recursos[i];
    if(var >=0 && var <= total[i]){
      count++;
    }
  }
  if (count == args-3) return 0; else return -1;
}

int requisicao_recursos(int pid, int recursos[]){

}

int main(int argc, char *argv[])
{

  int i, n;
  args = argc;
  srand(time(NULL));

  if(argc < 4){
      printf("Wrong args\n");
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
    total = malloc((argc-3)*sizeof(int));
    disponiveis = malloc((argc-3)*sizeof(int));
    for(i=0;i<(argc-4);i++){
      total[i] = atoi(argv[i+4]);
      disponiveis[i] = atoi(argv[i+4]);
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
  free(total);
  total=NULL;
  free(disponiveis);
  disponiveis=NULL;
  return 0;
}