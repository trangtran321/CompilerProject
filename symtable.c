/*
            Trang Tran
            October 20, 2023

            **Description & Purpose::
              This program creates a symbol table that is linked to our abstract symbol tree from the yacc file as well
              as the ast.c file. Each time a variable is declared or used in the user program,
              we can now check to see if the variable type matches what was previously declared, if variables
              are being used appropriately, whether or not variables have been previously declared or are being used
              without having been previously declared. This allows us to start building functionality to our compiler
              in helping us check errors in the inputted code segments.

              - Input: Variable names are inputted and compared with what is in the symbol table.
              - Output: Variables will either be added to the symbol table, or an error will be sent to the console.

*********************Changes made to document by TRANG TRAN on OCTOBER 23, 2023*********************
              -- Added check_params function to ensure that datatypes being used in a function call are appropriate and
              match what exists in the symbol table.
*/


#include <string.h>
#include "ast.h"
#include "symtable.h"

int GTEMP=0;  /* Global Temp counter */

// PRE:   Assume one up global variable GTEMP
// POST:  Returns string with the format _t%d and increments the global vairbal
// USAGE:   creates a variable name that is used to hold temporary, intermediate
//         values in the runtime enviroment

char * CreateTemp()
{    char hold[100];
     char *s;
     sprintf(hold,"_t%d",GTEMP++);
     s=strdup(hold);
     return (s);
}




/* Simple Insert into the symbol table with the size, type level that the name is being inserted into */

// PRE:  given elements for an item in the symbol table
//  POST:  Inserts an item in the symbol table list provided
//         the symbol does not exists at the level

struct SymbTab * Insert(char *name, enum DataTypes my_assigned_type, enum SYMBOL_SUBTYPE subtype, int  level, int mysize, int offset )
{
  struct SymbTab * n;
    n=Search(name,level, 0);
    if(n!=NULL)
      {
      printf("\n\tThe name %s exists at level %d already in the symbol table\n\tDuplicate can't be inserted\n",name, level);
      return (NULL);
      }
    else
    {
      struct SymbTab *p;
      p=malloc(sizeof(struct SymbTab));
      p->name=name;
      p->offset=offset;                   /* assign the offset */
      p->level=level;                     /* assign the level */
      p->mysize=mysize;                   /* assign the size */
      p->Declared_Type=my_assigned_type;  /* assign the Type */
      p->SubType=subtype;                 /* assign the Function  */
      p->next=NULL;

   //  Insert the record in the list ...
      if(first==NULL)
      {
        first=p;
      }
      else
      {
        p->next=first;
        first=p;
      }
      return (p);
    }

  printf("\n\tLabel inserted\n");
}

/* print out a single symbol table entry -- for debugging */
//  PRE:  Ptr to a symtabl structore
//  POST:  output to the screen in human readable form
void PrintSym(struct SymbTab *s)
{
         printf("\t%s\t\t%d\t\t%d\t%d\n",s->name,s->offset, s->level,s->Declared_Type);

}


/*  General display to see what is our symbol table */

//  PRE:  depends on global variable first
//  POST:  Formatted output of the symbol table

void Display()
{
   int i;
   struct SymbTab *p;
   p=first;
   printf("\n =====================================================================\n");
   printf("\t\t DISPLAY SYMBOL TABLE \n");
   printf("\n\tLABEL\t\tOffset \t\tLEVEL \ttype\n");
      while (p!=NULL)
      {
         PrintSym(p);
         p=p->next;
      }
  printf("\n =====================================================================\n\n\n");
}

/*  Search for a symbol name at level or below.  We have to do multiple passes into the symbol table because we have to find
   the name closest to us


  If recur is non-zero, then we look through all of the levels, otherwise, only our level
   We return a pointer to a SymbolTab structure so that we can use other functions/methods to get the attributes */




// PRE:   given a name and level and recure
//  POST:  returns NULL if not there, otherwise a PTR to the element in the table
// DETAIL:  search will stop at first level if recur set to 0
//          search will continue struct until level is 0 if recur is 1
struct SymbTab * Search(char name[], int level, int recur)
{
   int i,flag=0;
   struct SymbTab *p;

  /* for each level, try to find our symbol */
   while (level >= 0)
    {
       p=first;
       while (p!=NULL)
        {
         if((strcmp(p->name,name)==0) && (p->level == level))
           return p;
         p=p->next;
        }
       if (recur == 0) return (NULL);   /* we did not find it at our level */
       level--; /* check the next level up */
    }

   return  NULL;  /* did not find it, return 0 */
}

/* Remove all entries that have the indicated level
   We need to take care about updating first pointer into the linked list when we are deleting edge elements */

// PRE:  level
// POST:  removes all symbol table entrues with level equal to or higher

int Delete(int level)
{
    struct SymbTab *p,*f=NULL;  /* we follow with pointer f */
    int SIZE=0;
    p=first;

  /* cruise through the list */

    while (p != NULL)
      {
        /* do we match? */
        if (p->level>= level )
        { /* if it is the first in the list we have to update first, we know this by f being NULL */
           SIZE+=p->mysize;
           if ( f==NULL) first=p->next;
           else /* not the first element */
              {f->next=p->next;
              }
            p=p->next;

        }
        else
         {
               /* update follow pointer, move the p pointer */
                f=p;
                p=p->next;
          }

      }
    return(SIZE);
}

// PRE:  No input
// POST:  boolen if table has a PROTOTYPE in it
int Has_Proto()
{
   struct SymbTab *p = first;
   while (p != NULL)
    {
        if (p->SubType == SYM_FUNCTION_PROTO) return 1;
        p = p->next;
    }
   return 0;

}


/*  * @precondition: The formalP and actualP are not null
    * @postcondition: None
    * @param: None
	  * @return: Returns 1 if the formal and actual parameters match in datatype.
               Returns 0 if they do not.
    * @description: If the formal and actual parameters match in datatype the function
                    will return 1. The function uses several if-statements and recursion
                    to determine if the formal parameter pointer and actual parameter pointer
                    are compatible datatypes.
*/
int check_params( ASTnode *formalP, ASTnode *actualP){

  //if lists have the same length, return true
  if (formalP == NULL && actualP == NULL){
    return 1;
  } //endif

  //if one of the lists is shorter than the other, check to see if function
  //requires parameters
  if (formalP == NULL || actualP == NULL){
    //if the formal parameters are of type void and actual parameters mirror this,
    //return true.
    if (actualP == NULL && formalP -> datatype == A_VOIDTYPE){
      return 1;
    }//endif

    printf("Actual parameters do not match length of formal parameters!\n");
    return 0;
  } //endif

  if (formalP -> datatype != actualP -> datatype){
    printf("Parameter types do not match.\n");
    return 0;
  } //endif

  if (formalP -> symbol -> SubType == SYM_SCALAR){
    //if s1 is not null, this is an array.
    //we need to figure out if s1 has been instantiated & if the index is permissible
      if (actualP -> s1 -> nodetype == A_VAR){
        if(var_check(actualP) == 0){
        printf("Variable check failed in check_params.\n");
        return 0;
        }
      }
  }

  if (formalP -> symbol -> SubType == SYM_ARRAY){
    //if actual arguments do not have an expression attached and it is an A_VAR node, it is not referring to the list
    //of ints, etc. It is likely it is referring to a specific index in an array.
    if (actualP -> s1 -> nodetype == A_NUM){
      printf("Invalid input of number in array.\n");
      return 0;
    }

    if(actualP -> s1 -> symbol -> SubType != SYM_ARRAY){
      printf("Actual variable does not refer to an array-type variable.\n");
      return 0;
    }
  }

  return check_params(formalP->s2, actualP->s2);

}

/*  * @precondition: p is a node that is not null
    * @postcondition: None
    * @param: None
	  * @return: Returns 1 if the variable is being properly used.
               Returns 0 if it is not.
    * @description: p is a pointer to an ASTnode. var_check() verifies that the variable is being
                    used appropriately according to the values set inside the symbol table.
*/
int var_check(ASTnode *p){
  //if the node is an A_VAR node, check to make sure the subtype matches
  //and the array index is appropriate

  if (p->nodetype == A_VAR){
    //if var is an array, check if the size is right.
    if (p -> symbol -> SubType == SYM_ARRAY){
      if (p -> s1 == NULL){
        printf("Name of Array being misused: %s\n", p->symbol->name);
        printf("Illegal use of array. Need to input an index.\n");
        return 0;
      }

      return 1;
	  }//endif

    if (p->symbol -> SubType == SYM_SCALAR){
      if (p -> s1 != NULL){
        printf("Name of Array being misused: %s\n", p->symbol->name);
        printf("An array was passed illegally.\n");
        return 0;
      }
      return 1;
    }//endif

    if (p -> symbol -> SubType == SYM_FUNCTION){
	  	printf("Illegal use of functions\n");
      return 0;
	  }//endif
  }
  return 1;
}


/*  * @precondition: func and proto are ASTnode pointers
    * @postcondition: None
    * @param: None
	  * @return: Returns 1 if the function and prototypes match
               Returns 0 if they do not.
    * @description: This function will iterate through several if statements to check whether or not the
                    function and prototype have the same name, datatype, subtype and the list of parameters are the
                    same.
*/
int proto_func_check(ASTnode *func, ASTnode *proto){

  //if lists have the same length, return true
  if (proto == NULL && func == NULL){
    return 1;
  } //endif

  //if lists are not the same length
  if (proto == NULL || func == NULL){
    printf("Function and prototype parameters do not match in length.\n");
    return 0;
  }

  //if parameters are not of the same datatype, return fals
  if (proto->datatype != func -> datatype){

    printf("Function and prototype parameter datatypes do not match.\n");
    return 0;
  }

  //if prototype and function do not have same subtype
  if (proto-> value != func -> value){
    printf("Function and prototype are not the same subtype.\n");
    return 0;
  }

  if (proto->datatype != A_VOIDTYPE && func->datatype != A_VOIDTYPE){
    //if prototype and function do not have the same name
    if ((strcmp(proto->name,func->name)!=0)){
      printf("Function and prototype need to have the same name.\n");
      return 0;
    }
  }

  return proto_func_check(proto->s2, func->s2);
}


