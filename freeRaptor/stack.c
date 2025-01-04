#include "stack.h"
stack* stack_build(){
  stack* out = (stack*) malloc(sizeof(stack));
  out->len   = 0;
  out->size  = 10;
  out->arr   = (int*) malloc(sizeof(int)*out->size);
  return out;
}

void stack_increament(stack* stk){
  stk->size = stk->size * 2;
  stk->arr = realloc(stk->arr, sizeof(int)*stk->size);
  return;
}

bool stack_isfull(stack* stk){
  return stk->len == stk->size;
}

bool stack_ifempty(stack* stk){
  return stk->len==0;
}

void stack_push(stack* stk,int x){
  if(stk->len == stk->size)stack_increament(stk);
  stk->arr[stk->len++]=x;
  return;
}

int stack_pop(stack* stk){
  if(stack_ifempty(stk))return -1;
  return stk->arr[--stk->len];
}

void stack_print(stack* s){
  for(int i=0;i<s->len;i++)printf("%d ",s->arr[i]);
  printf("\n");
}

int main1(){
  int a[] = {98,3,6,7,9,32,54,1};
  int n = sizeof(a)/sizeof(a[0]);
  stack* s = stack_build();
  for(int i=0;i<n;i++){
    stack_push(s,a[i]);
  }
  stack_print(s);
  for(int i=0;i<n;i++)printf("%d ",stack_pop(s));
  printf("\n");
  return 0;
}
