#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include "lexer.c"
#include "parser.c"
#include "vm.h"
#include "vm.c"
//#include "read_2.c"
#include "eval-apply.c"
#include "print.c"
#include "compiler.c"


int main(char *argc, char **argv[]){

  char str[20];
  struct pair *list = NULL;
  struct  token_list *token_list = NULL;
  struct  token_list *token_list2 = NULL;
  struct token_object object1;
  struct object *expr_list;
  struct object *expr;
  struct object *expr2;
  struct object *result_expr = NULL;
  object* code_tree = NULL;
  char* operator = "third";
  char* string = "\"A string\"";
  int b = 20, c = 30;
  // hashtable_t *env = ht_create( 65536 );
  char* result = NULL;

  object1.type = "left_paren";
  object1.value = "(";
  token_list = prepend_token(object1, token_list);

  object1.type = "primitive";
  object1.value = "+";
  token_list = prepend_token(object1, token_list);

  object1.type = "num";
  object1.value = "137";
  token_list = prepend_token(object1, token_list);

  object1.type = "num";
  object1.value = "349";
  token_list = prepend_token(object1, token_list);

  object1.type = "right_paren";
  object1.value = ")";
  token_list = prepend_token(object1, token_list);


  //token_list = reverse_token_list(token_list);
  //print_token_list(token_list);
  
  while(token_list != NULL){
    if(strcmp(token_list->val.type,"right_paren")==0){
      //indicates the start of a new list
      expr_list = NULL;
    }else if(strcmp(token_list->val.type,"left_paren") == 0){
      //end of a list
    }else if(strcmp(token_list->val.type,"primitive")==0){
      //constructing an operator onto the list
      expr2 = create_primitiveop(token_list->val.value);
      expr_list = cons(expr2, expr_list); 
    }else if(strcmp(token_list->val.type,"num")==0){
      //constructing a number onto the list
      expr2 = create_number(token_list->val.value);
      expr_list = cons(expr2, expr_list);
    }else{
      printf("invalid token");
    }
    token_list = token_list->next;
  }

  print(eval(expr_list,token_list));

  
  // print(eval(parse(lexer("(+ 137 349)"))code_tree));
  
  /* while (1){     
     printf("repl>");
     fgets (str, 20, stdin);
     //printf("=>");
     //micro_read(str);
     printf("=>%s\n", eval(read(str)));
     /*char str[20];
     while (1){     
     printf("repl>");
     fgets (str, 20, stdin);
     printf("=>%s\n", eval(read(str)));
     }
     }*/
  return 0;
}//end of main>

