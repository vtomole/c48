//https://llvm.org/docs/tutorial/LangImpl01.html#language
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
struct object{
  char* type;
  char* value;
}object;

struct identifier{
  char* identifier_token;
  int length;
}identifier;

typedef struct token_list{
  struct object val;
  struct token_list *next;
}token_list;

//Chop off first N characters
char * chopN(char * charBuffer, int n )
{   
  return charBuffer + n;   
}


token_list* reverse_tokenlist(token_list* head)
{
  token_list* prev    = NULL;
  token_list* current = head;
  token_list* next;
  while (current != NULL)
    {
      next  = current->next;
      current->next = prev;
      prev = current;
      current = next;
    }
  head = prev;
  return head;
}

token_list* cons1(struct object val, struct token_list *cdr)
{
 
  //pair* pair = (pair*)malloc(sizeof(pair));
  token_list* pair = malloc(sizeof(pair));
  if(pair == NULL)
    {
      printf("Error creating a new node.\n");
      exit(0);
    }
  pair->val = val;
  pair->next = cdr;
 
  return pair;
}

enum Token {
  tok_eof = -1,
  tok_def = -2,
  tok_identifier = -4,
  tok_number = -5,
};

static char* indentifier_string;
static double number_value;


int count_token_list (token_list* cursor){
  int c = 0;
  while(cursor != NULL){
    c++;
    cursor = cursor->next;
  }
  return c;
}


char* first (struct token_list *list){
  if(list){
    return list->val.value;
  }
  return 0;
}
char* find_value (struct token_list *list){
  if(list){
    return list->val.value;
  }
  return 0;
}
char* find_type (struct token_list *list){
  if(list){
    return list->val.type;
  }
  return 0;
}

token_list* rest( struct token_list *list){
  return list ->  next;
  
}

//https://stackoverflow.com/questions/14259540/c-programming-strcat-using-pointer
char *scat(char *s,char *t)
{
    char *p=malloc(strlen(s)+strlen(t)+1);    /* 3: you will have to reserve memory to hold the copy. */
    int ptr =0, temp = 0;                   /* 4 initialise some helpers */

    while(s[temp]!='\0'){                  /* 5. use the temp to "walk" over string 1 */
        p[ptr++] = s[temp++];
    }
    temp=0;
    while(t[temp]!='\0'){                   /* and string two */
        p[ptr++]=t[temp++];
    }
    return p;
}

char* print_token_list(struct token_list *list, char* result){
  if(result == NULL){
    result = first(list);
    print_token_list(rest(list), result);
   
  }
  else if(list && result !=NULL){
     
    result = scat(result, first(list));
    //printf("The result %s\n", result);
    // printf("%s\n", first(list));
    print_token_list(rest(list), result);

  }
  else{
  return result;
  }

}

char* print_token_list_debug(struct token_list *list, char* result){
  if(result == NULL){
    result = ("[");
    result = scat(result,find_type(list));
    result = scat(result,",");
    result = scat(result, find_value(list));
    result = scat(result,"]");
    print_token_list_debug(rest(list), result);

   
  }
else if(list && result !=NULL){
    result = scat(result,"[");
    result = scat(result, find_type(list));
    result = scat(result,",");     
    result = scat(result, find_value(list));
    result = scat(result,"]");
    print_token_list_debug(rest(list), result);
  }
  else{
 return result;
  }
}


int iswhitespace (char c){
  if(c == '\n' || c == ' ' ){
    return 1;
  }
  else{
    return 0;
  }
}

char*  append(char* s, char c)
{

  int len = strlen(s);

  printf("Char s %s\n", s);

  printf("Length %c\n", s[1]);
	
	 
  s[len-1] = c;
  printf("In append\n");
	
  s[len+1] = '\0';

  return s;
}



struct identifier* read_identifier (char* program, int index){
  struct identifier *identifier1;
  
  const char s[2] = " ";
  
  program = chopN( program, index );
  
  char *string = strdup(program);
  printf("the string %s\n", string);
  
  identifier1->identifier_token= strtok(string, s);
  printf("In read_identifier\n");
  identifier1->length = strlen(identifier1->identifier_token);
  if(identifier1 == NULL){
    printf("It's null\n");
  }
  return identifier1;

}

struct identifier* read_number (char* program, int index){
  struct identifier *identifier1;
  const char s[2] = " ";
  program = chopN( program, index );
  char *string = strdup(program);
  identifier1->identifier_token= strtok(string, s);
  identifier1->length = strlen(identifier1->identifier_token);
  
  return identifier1;

}
//Lexer



char* token_type (char* token){

  if (isalpha(token[0])){
    return "symbol";
  }
  else if (isdigit(token[0])){
    return "num";
  }
  else{
    return "identifier";
  }


}

int count_chars(char* string, char ch)
{
  int count = 0;
  int i;

  // We are computing the length once at this point
  // because it is a relatively lengthy operation,
  // and we don't want to have to compute it anew
  // every time the i < length condition is checked.
  int length = strlen(string);

  for (i = 0; i < length; i++)
    {
      if (string[i] == ch)
        {
	  count++;
        }
    }

  return count;
}

//TODO: check for Booleans, Characters, Strings
token_list* list_lexer (char *program){
  static int last_character = ' ';
  int i;
   
  struct  token_list *token_list = NULL;
  struct object object1;
  struct identifier *id;
  const char s[2] = " ";
  char *token;
  int num_right; 

  const char *right_par = ")";
   
  program = strdup(program);
  /* get the first token */
  token = strtok(program, s);

 
  /* walk through other tokens */
  while( token != NULL ) 
    {
      num_right = count_chars(token, ')');
	
      if(token[0] == '('){
	 
	memmove(token, token+1, strlen(token));


	object1.type = "left_paren";
	object1.value = "(";
	token_list = cons1(object1, token_list);
	object1.type = token_type(token);
	object1.value = token;
	token_list = cons1(object1, token_list);

	  
	  
      }
      else if ( num_right>= 1){ 


	    
	token[strlen(token)-num_right] = 0;
	 
	object1.type = token_type(token);
	object1.value = token;
	 
	token_list = cons1(object1, token_list);

	object1.type = "right_paren";
	object1.value = ")";
	token_list = cons1(object1, token_list);
	
	 
	for(i =1; i < num_right; i++){
	    
	     
	  object1.type = "right_paren";
	  object1.value = ")";
	  token_list = cons1(object1, token_list);

	}
	    
	 
     
	 
      }
      else{
	object1.type = token_type(token);
	object1.value = token;
	token_list = cons1(object1, token_list);

      }
    
      token = strtok(NULL, s);  

    }

  if(token_list == NULL){

    object1.type = "identifier";
    object1.value = "open_paren";
    token_list = cons1(object1, token_list);

  }

   


  return  reverse_tokenlist(token_list);
}





//TODO: check for Booleans, Characters, Strings
token_list* list_lexer_tmp (char *program){
  static int last_character = ' ';
  int i;
   
  struct  token_list *token_list = NULL;
  struct object object1;
  struct identifier *id;

  object1.type = "identifier";
  object1.value = "open_paren";
  token_list = cons1(object1, token_list);
  
  /* object1.type = "operator";
     object1.value = "+";
     token_list = cons1(object1, token_list);
  
     object1.type = "identifier";
     object1.value = "open_paren";
     token_list = cons1(object1, token_list);
  
     object1.type = "operator";
     object1.value = "+";
     token_list = cons1(object1, token_list);
  
     object1.type = "num";
     object1.value = "2";
     token_list = cons1(object1, token_list);
     object1.type = "num";
     object1.value = "3";
     token_list = cons1(object1, token_list);
  
     object1.type = "identifier";
     object1.value = "closed_paren";
     token_list = cons1(object1, token_list);
  
     object1.type = "num";
     object1.value = "7";
     token_list = cons1(object1, token_list);
 
     object1.type = "identifier";
     object1.value = "closed_paren";*/


  //printf("is alpha-test char %c\n",program[0]);
  //printf("is alpha-test %d\n",isalpha(program[0])); 
  for(i =0; i < strlen(program); i++){
    //Check if it's whitespace
    if(iswhitespace(program[i])){
	
    }
    //check if it is an open parentheses
    else if (program[i] == '('){
      object1.type = "identifier";
      object1.value = "open_paren";
      token_list = cons1(object1, token_list);

    }
    //check if it is a closed parentheses
    else if (program[i] == ')'){
      object1.type = "identifier";
      object1.value = "closed_paren";
      token_list = cons1(object1, token_list);

    }    
    //check if its in the alphabet
    else if (isalpha(program[i])){
      
      object1.type = "symbol";
      id =  read_identifier(program, i);
      object1.value = id->identifier_token;
      program = chopN( program, id->length ); 
      printf("IDENTIFIER %s\n", object1.value);
      token_list = cons1(object1, token_list);

    }
    //check if its a number
    else if(isdigit(program[i])){
      object1.type = "num";
      id =  read_number(program, i);
      object1.value = id->identifier_token;
      program = chopN( program, id->length ); 
      printf("NUMBER VALUE %s\n", object1.value);
      token_list = cons1(object1, token_list);

    }
    //invalid syntax
    else{
      printf("Illegal syntax\n");
      
    }
  }
  printf("The number %d\n", count_token_list(token_list));
  //print_token_list(reverse_token_list(token_list));
  return token_list;
}

