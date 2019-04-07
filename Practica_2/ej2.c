#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

//Para calcular tiempo
double dwalltime(){
  double sec;
  struct timeval tv;

  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}

int N, NUM_THREADS, NUM;
int* vector;
int* result;
int block_size;

void* buscar(void* id){
  int block = *((int*)id);
  result[block] = 0;
  for(int i = (block*block_size); i < ((1+block)*block_size); i++){
    if(vector[i] == NUM) result[block]++;
  }
  pthread_exit(0);
}

int main(int argc, char* argv[]){

  //Controla los argumentos al programa
  if ((argc != 3) || ((N = atoi(argv[2])) <= 0) )
  {
    printf("Error en cantidad de parametros");
    exit(1);
  }

  srand(time(NULL));

  double timetick;
  NUM_THREADS = atoi(argv[1]);
  N = atoi(argv[2]);
  NUM = rand() % 5;
  block_size = N/NUM_THREADS;
  
  int ids[NUM_THREADS];  
  result = (int*)malloc(sizeof(int)*NUM_THREADS);
  vector = (int*)malloc(sizeof(int)*N);

  for(int i = 0; i < N; i++){
    vector[i] = rand() % 5;
  }

  pthread_attr_t attr;
  pthread_t threads[NUM_THREADS];
  pthread_attr_init(&attr);
  
  timetick = dwalltime();

  //Lanza los threads para que realicen el calculo

  for(int i = 0; i < NUM_THREADS; i++){
    ids[i] = i;
    pthread_create(&threads[i], &attr, buscar, &ids[i]);
  }
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }

  //Calcula el resultado final sumando lo que calcularon los threads

  int res;
  for(int i = 0; i < NUM_THREADS; i++) res+= result[i];

  float time = dwalltime() - timetick;
  printf("%f\n", time);

}