

typedef struct constructor_cell{
  void* car; //first element
  void* cdr; //rest of the list w/ first element
}constructor_cell;

//constructor_cell* code_tree = NULL; //Put the cells in this

typedef struct type_list{
  struct object val;
  struct type_list *next;
}type_list;

//fine
char* get_car(void* car){
  return *(char**)car;
}
  
//Cell constructor
constructor_cell* cons2(void* first_element, void* list){

  constructor_cell* cell = malloc(sizeof(constructor_cell));
  if(cell == NULL){
    printf("Error creating a new node.\n");
    exit(0);
  }
  cell->car = first_element;
  cell->cdr = list;
  return cell;
}

//recursively adds the token_lists to the code tree
constructor_cell* parse(token_list* token_list, constructor_cell* code_tree){
  if(token_list == NULL){
    return code_tree;
  }
  code_tree = cons2(&token_list, &code_tree);
  //printf("%s", get_car(code_tree->car));
  parse(token_list->next, code_tree);
}
