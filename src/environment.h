typedef enum {EMPTY_LIST, BOOLEAN, SYMBOL, FIXNUM, FLOAT,
              CHARACTER, STRING, PAIR, PRIMITIVE_PROC,
              COMPOUND_PROC} object_type;

typedef enum {MATH, EQUALITY, COMPARISON, CHAR} primitive_type;

typedef struct object{  
  object_type obj_type;
  struct cons_cell{
    struct object *car;
    struct object *cdr;
  }cons_cell;
 
  char* symbol;
  char *string;
  char character;
  long number;
  //enum boolean empty_list;
  int boolean;
  double decimal;
  
  struct primitive_proc{
    struct object *(*fn)(struct object *arguments);
    primitive_type prim_type;
    struct object* acceptables;
  }primitive_proc;

  struct compound_proc {
    struct object *parameters;
    struct object *body;
    struct object *env;
  }compound_proc;
  
}object;



object* cons(object *car, object *cdr);
void  initialize_environment(void);
