//#include "parser.h"

//struct symbol{//typedef struct symbol???
//  char *name;
//  struct symbol *next;
//}symbol;

enum boolean {true, false};

typedef struct object{
  char* type;
  struct cons_cell{
    struct object *car;
    struct object *cdr;
  }cons_cell;
 
  char* variable;
  char *string;
  int number;
  enum boolean boolean;
}object;

//global variables
int parse_count = 0;

object *create_object(char *value, char *type){
  object *obj = malloc(sizeof(obj));
  obj->type = type;
  obj->variable = value;
return obj;
}

/**
 *This function constructs ????
 * Parameters:
 * - car, the first object in the code tree
 * - cdr, the rest of the objects in the code tree
 * Return Value:
 * - test1, an object ???
 */
object* cons(object *car, object *cdr){
  struct object *object1;
  object *test1 = malloc(sizeof(*object1));

  test1->type = "cons";
  test1->cons_cell.car = car;
  test1->cons_cell.cdr = cdr;

  return test1;
}

/**
 *This function checks if the cells car is ???
 * Parameters:
 * - cell, an object ???
 * Return Value:
 * - ???
 */
object* car(object *cell){
  //  assert (strcmp(cell->type, "cons") == 0);
  return cell->cons_cell.car;
}

/**
 *This function checks that the cells cdr is ???
 * Parameters:
 * - cell
 * Return Value:
 * - ???
 */
object* cdr(object *cell){
  assert (strcmp(cell->type, "cons") == 0);
  return cell->cons_cell.cdr;
}

/**
 *This function creates an object representing a number
 * Parameters:
 * - number, the value of the object to create
 * Return Value:
 * - num an object with type number and value number
 */
object* create_number(char* number){
  struct object *object1;
  object *num = malloc(sizeof(*object1));
  num->type  ="number";
  int temp = atoi(number);
  num->number = temp;
  return num;
}

/**
 *This function creates an object representing a number
 * Parameters:
 * - number, the value of the object to create
 * Return Value:
 * - num an object with type number and value number
 */
object* create_string(char* string){
  struct object *object1;
  object *str = malloc(sizeof(*object1));
  str->type  ="string";
  str->string = string;
  return str;
}

/**
 *This function creates an object representing a variable
 * Parameters:
 * - variable the value of the object to create
 * Return Value:
 * -var, an object with type "variable", and value variable
 */
object* create_variable(char* variable){
  struct object *object1;
  object *var = malloc(sizeof(*object1));
  var->type = "variable";
  //var->value = variable;
  var->variable = variable;
  return var;
}

object* create_boolean(char* variable){
  struct object *object1;
  object *var = malloc(sizeof(*object1));
  var->type = "boolean";
  //var->value = variable;
  var->variable = variable;
  return var;
}

/**
 *This function creates an object representing a primitive operation
 * Parameters:
 * - variable the value of the object to create
 * Return Value:
 * -var, an object with type "primeop", and value variable
 */
object* create_primitiveop(char* variable){
  struct object *object1;
  object *var = malloc(sizeof(*object1));
  var->type = "primitive";
  //var->value = variable;
  var->variable = variable;
  return var;
}

//constructor_cell* code_tree = NULL; //Put the cells in this

typedef struct type_list{
  struct object val;
  struct type_list *next;
}type_list;

/**Use this when you dereference void pointer to pointer fine
 *This function gets the head of the code tree
 * Parameters:
 * -car, the cell containing the the type and value of the head of the code tree
 * Return Value:
 * -car, a c string of the value of the head of the code_tree
 */
char* get_car(void* car){
  return *(char**)car;
}


int count_objects(object* cursor){
  int c = 0;
  while(cursor != NULL){
    c++;
    cursor = cursor->cons_cell.cdr;
  }
  return c;
}


object* create_object_for_parse(char* type, char* value){

  // printf("DA TYPE %s\n", type);
   if(strcmp(type,"primitive")==0){
     return  create_primitiveop(value);
   } else if(strcmp(type,"variable")==0){
     return create_primitiveop(value);
   } else if(strcmp(type,"num")==0){
     return create_number(value);
   } else if(strcmp(type,"string")==0){
     return create_string(value);
   } else if(strcmp(type,"boolean")==0){
     return create_boolean(value);
   } else {
     printf("invalid token\n");
     exit(0);
   }
 }


/**
 *Recursivly adds tokens to the code_tree
 * Parameters:
 * -token_list, the list of tokens to add to the code_tree
 * -code_tree, the code_tree being built
 */

//Will return object list
/*
 * this method recursivly goes through parse to create an unknown number of lists
 * by doing this each call to parse_rec should be a nested statement
 * each statement returns a list containing all the statements before a right or left paren
 * no seg_faults however the code isn't doing anything visible with the temp_list it creates and maintains.
**/

object* parse(token_list* token_list, object* expr){
  object*  expr2;
 
  printf("\nToken val: %s\n", token_list->val.value);
  print_token_list_value(token_list);

  if(strcmp(token_list->val.type,"right_paren")==0){
    token_list = token_list->next;
    expr2 = parse(token_list, expr);//recurse 
    printf("hi");
    expr = cons(expr2, expr);//attach

  }else if(strcmp(token_list->val.type,"left_paren")==0){
    printf("\n");
    return expr;//exit recurse

  }else if(strcmp(token_list->val.type,"primitive")==0){
    expr2 = create_object_for_parse(token_list->val.type,token_list->val.value);
    expr = cons(car(expr2), cons(expr2, expr));

  }else{
    //add expression or prim to list
    expr2 = create_object_for_parse(token_list->val.type,token_list->val.value);
    expr = cons(expr2,expr);//attach previous to this car

  }
  if(token_list->next)
    token_list=token_list->next;
    expr = parse(token_list, expr);
  return expr;
}


object* parse_tmp(token_list* token_list){
  struct object *node;
  struct object *branch;
  struct object *expression;

  while(token_list){
    print_token_list_value(token_list);
    printf("\n");

    if(strcmp(token_list->val.type,"right_paren")==0){
      branch = NULL;//start of a new cons list 
    }else if(strcmp(token_list->val.type,"left_paren")==0){
      printf("hello\n");
      expression = cons(branch,expression);
    }else if(strcmp(token_list->val.type,"primitive")==0){
      printf("test\n");

      node = create_object_for_parse(token_list->val.type,token_list->val.value);
      branch = cons(node, cons(node, branch));
    }else{
       node = create_object_for_parse(token_list->val.type,token_list->val.value);
       branch = cons(node,branch);//attach previous to this car
    } 
    
    token_list=token_list->next;
  }
  /* if(expression){
    printf("%s\n",(car(car(expression)))->variable);
    printf("%s\n",(car(cdr(car(expression))))->variable);
    printf("%d\n",(car(cdr(cdr(car(expression)))))->number);
    printf("%d\n",(car(cdr(cdr(cdr(car(expression))))))->number);
    }*/

    return expression;
}

