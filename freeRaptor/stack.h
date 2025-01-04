#ifndef STACK_H
#define STACK_H

#include "stdio.h"
#include"stdlib.h"
#include"stdint.h"
#include"stdbool.h"
typedef struct{
  uint32_t size;
  uint32_t len;
  int* arr;
}stack;

stack* stack_build();
void stack_increament(stack* stk);
bool stack_isfull(stack* stk);
bool stack_ifempty(stack* stk);
void stack_push(stack* stk,int x);
int stack_pop(stack* stk);
void stack_print(stack* s);
#endif // !STACK_H
