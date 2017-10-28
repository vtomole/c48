#include "stdio.h"
#include "../src/comp_front.c"

void stack_test(){
  char str[25];
  char *a = "+";
  char *b = "4";
  char *c = "5";
  sprintf(str, "( %s %s %s )", a, b, c);

  stack *s = create_stack();
  
  push(s, (void*)"working");
  push(s, (void*)"is");
  push(s, (void*)"world");
  push(s, (void*)"hello");
  push(s, (void*)str);
  

  void *rtrn = pop(s);
  while(rtrn != NULL){
    printf("%s\n", (char*)rtrn);
    rtrn = pop(s);
  }

  push(s, (void*)1);
  push(s, (void*)2);
  push(s, (void*)3);
  push(s, (void*)4);
  push(s, (void*)5);

  rtrn = pop(s);
  while(rtrn != NULL){
    printf("%d\n", (int)rtrn);
    rtrn = pop(s);
  }
}

void queue_test(){
  queue *q = create_queue();
  
  enqueue(q, (void*)"hello");
  enqueue(q, (void*)"world");
  enqueue(q, (void*)"is");
  enqueue(q, (void*)"working");

  void *rtrn = dequeue(q);
  while(rtrn != NULL){
    printf("%s\n", (char*)rtrn);
    rtrn = dequeue(q);
  }

  enqueue(q, (void*)1);
  enqueue(q, (void*)2);
  enqueue(q, (void*)3);
  enqueue(q, (void*)4);
  enqueue(q, (void*)5);

  rtrn = dequeue(q);
  while(rtrn != NULL){
    printf("%d\n", (int)rtrn);
    rtrn = dequeue(q);
  }

}

void postfix_test(){
  char *str = "( 7 - 4  + 5 )";
  char *res = postfix(str); 
  printf("postfix_test: %s\n", res);
}

void prefix_test(char *expr){
  char *res;
  res = prefix(expr);
  printf("prefix_test: %s\n", res);
  
}


int main(){
  
  // stack_test();
  // queue_test();
  // prefix_test("( 7 + 4 - 5 )");     // should be ( - ( + 7 4 ) 5 )
  // prefix_test("( 7 + ( 4 - 5 ) )"); // should be ( + 7 ( - 4 5 ) )
  // prefix_test("( 7 * 4 - 5 )");     // should be ( - ( * 7 4 ) 5 )
  // prefix_test("( 7 + 4 * 5 )");     // should be ( + 7 ( * 4 5 ) )
  prefix_test("( 7 / 4 * 5 )");     // should be ( * ( / 7 4 ) 5 )

  return 0;
}