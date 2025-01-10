// #include "./raptor_header/base.h"
#include"unistd.h"
#include"stdio.h"
#include"pthread.h"
int pthread_tryjoin_np(pthread_t thread, void **retval);
void* func(void* x){
  printf("in function %d\n",*(int*)x);
  sleep(3);
  printf("in function %d\n",*(int*)x);
}
int main(){
  pthread_t thread;
  pthread_create(&thread,NULL,func,NULL);
  int res;

  while((res = pthread_tryjoin_np(thread,NULL))==0)printf("running\n");
  printf("done\n");
  return 0;
}
