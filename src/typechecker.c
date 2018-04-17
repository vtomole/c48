#include "eval-apply.h"

object *eval_assignmenttp(object *exp, object *env) {
    set_variable_value(assignment_variable(exp), 
                       eval(assignment_value(exp), env),
                       env);
    //print(exp);
    return exp;
}

object *goto_end (object *exp) {
  object *prev;
  object *next;

  next = exp;

  while (!is_the_empty_list(next)) {
    prev = car(next);
    next = cdr(next);
  }
  //print(prev);

  return exp;

}

object *eval_definitiontp(object *exp, object *env) {
    define_variable(definition_variable(exp), 
                    eval(definition_value(exp), env),
                    env);
    return exp;
}


object *typecheck(object *exp, object *env) {
    object *procedure;
    object *arguments;
    object *type_object;
    object *primary_type;
    object *acceptables;
    object *type_arguments;
    object* static_exp;
    object *end_list;

    static_exp = exp;

tailcall:
    if (self_evaluatingp(exp)) {
        return static_exp;
    }
    else if (variablep(exp)) {
      return lookup_variable_value(exp, env);
    }
    else if (is_quoted(exp)) {
        return static_exp;
    }
    else if (is_assignment(exp)) {
      //printf("It's an assignment \n");
      return eval_assignmenttp(exp, env);
    }
    else if (is_definition(exp)) {
      //printf("It's define \n");
        return eval_definitiontp(exp, env);
    }
    else if (is_if(exp)) {
      //printf("It's if \n");
        exp = is_true(eval(if_predicate(exp), env)) ?
                  if_consequent(exp) :
                  if_alternative(exp);
        goto tailcall;
    }
    else if (is_lambda(exp)) {
      //printf("It's lambda \n");
        return make_compound_proc(lambda_parameters(exp),
                                  lambda_body(exp),
                                  env);
    }
    else if (is_begin(exp)) {
      //printf("It's begin \n");
        exp = begin_actions(exp);
        while (!is_last_exp(exp)) {
            eval(first_exp(exp), env);
            exp = rest_exps(exp);
        }
        exp = first_exp(exp);
        goto tailcall;
    }
    else if (is_cond(exp)) {
      //printf("It's cond \n");
        exp = cond_to_if(exp);
        goto tailcall;
    }
    else if (is_let(exp)) {
      //printf("It's let \n");
        exp = let_to_application(exp);
        goto tailcall;
    }
    else if (is_application(exp)) {
      //printf("It's apply \n");
      procedure = eval(operator(exp), env);
        arguments = list_of_values(operands(exp), env);
	type_arguments = list_of_values(operands(exp), env);
        if (primitivep(procedure)) {
	  acceptables = procedure->primitive_proc.acceptables;
	  
	  primary_type = car(cdr(type_arguments));
	  
	  while (!is_the_empty_list(type_arguments)) {
	    if(primary_type->obj_type != car(type_arguments)->obj_type){
	      fprintf(stderr, "Type error expression\n");
	      exit(1);
	    }	      
	    type_arguments = cdr(type_arguments);
	  }
	  
	  type_arguments = list_of_values(operands(exp), env);

	  int type_flag = 0;

	  while (!is_the_empty_list(acceptables)) {
	    
	     if(primary_type->obj_type == car(acceptables)->number){
	       type_flag = 1;
	      }	     
	    acceptables = cdr(acceptables);
	    }
	  
	  if(type_flag == 0){
	    fprintf(stderr, "Type error variables\n");
	    exit(1);
	  }

	  //printf("Static exp\n");
	  //print(static_exp);
          return static_exp;
       
        }
        else if (is_compound_proc(procedure)) {
	  
            env = extend_environment( 
                       procedure->compound_proc.parameters,
                       arguments,
                       procedure->compound_proc.env);
            exp = make_begin(procedure->compound_proc.body);
	    
            goto tailcall;
        }
        else {
            fprintf(stderr, "unknown procedure type\n");
            exit(1);
        }
        
    }
    else {
        fprintf(stderr, "cannot eval unknown expression type\n");
        exit(1);
    }
    fprintf(stderr, "eval illegal state\n");
    exit(1);
}
