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
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int done = 0;

int *total;
int *disponiveis;
int args;

void *process(void* tpid)
{
  const int pid = (long) tpid;

  int i, var, nextpid = pid;
  int *recursos;

  recursos = calloc((args-3), sizeof(int));

  while(1){
    pthread_mutex_lock(&lock);
    printf("Thread PID %d requisita: ", pid);
    for(i=0;i<(args-4);i++){
      recursos[i] = (int) rand() % (total[i]+1);
      printf("%d ", recursos[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&lock);

    var = requisicao_recursos(pid, recursos);
    if(var == 0){
      sleep(3);
      libera_recursos(pid, recursos);
      sleep(3);
    }
    else{
      printf("Thread PID: %d não esta em um estado seguro, ABORTING THREAD...\n", pid);
      done ++;
      break;

    }

  }

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
  if (count == i) return 0; else return -1;
}

int requisicao_recursos(int pid, int recursos[]){
  int i;
  if(safe_state(recursos) == 0){
    pthread_mutex_lock(&lock);
    printf("Thread PID: %d requisitou recursos com segurança, DISPONIVEIS:", pid);
    for(i=0;i<(args-4);i++){
      disponiveis[i] = disponiveis[i] - recursos[i];   
      printf("%d ", disponiveis[i]);
    }
    printf("\n");
    pthread_mutex_unlock(&lock);
    return 0;
  }
  else{
    return -1;
  }
}

int libera_recursos(int pid, int recursos[]){
  int i;
  printf("Thread PID: %d liberou recursos, DISPONIVEIS:", pid);
  pthread_mutex_lock(&lock);
  for(i=0;i<(args-4);i++){
    disponiveis[i] = disponiveis[i] + recursos[i];   
    printf("%d ", disponiveis[i]);
  }
  pthread_mutex_unlock(&lock);
  printf("\n");
  return 0;
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
    if(pthread_create(&thread_id[i], NULL, process, (void*)(long)i)) {
      fprintf(stderr, "Error creating thread\n");
      return 1;
    }

  }

  pthread_mutex_lock(&lock);

  while(done<args-4 ){
      pthread_cond_wait(&cond,&lock);
      break;
    }

  pthread_mutex_unlock(&lock);


  free(total);
  total=NULL;
  free(disponiveis);
  disponiveis=NULL;
  return 0;
}