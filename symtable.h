/*  Symbol Table --linked list  headers
    Used for Compilers class

    Modified Spring 2015 to allow for name to pointed to by symtable, instead of copied, since the name is copied
    into the heap

    Modified to have levels.  A level 0 means global variable, other levels means in range of the function.  We
    start out our offsets at 0 (from stack pointer) for level 1,,,when we enter a functional declaration.
    We increment offset each time we insert a new variable.  A variable is considered to be valid if it is found in
    the symbol table at our level or lesser level.  If at 0, then it is global.

    We return a pointer to the symbol table when a variable matches our creteria.

    We add a routine to remove variables at our level and above.
---------------------------------------------------------------------------------------------------------
    Trang Tran
        October 1, 2023
        Lab 6 : ast.c

        **Description & Purpose::
            This header file connects the abstract symbol tree with the symbol table.The SymbTab nodes all
            represent a line in the symbol table and are either variables,functions, or prototypes. They connect
            to the ASTnode via the fparms variable, which is a pointer to a specific node.

    *********************Changes made to document by TRANG TRAN on OCTOBER 23, 2023*********************
              -- Added check_params function to ensure that datatypes being used in a function call are appropriate and
              match what exists in the symbol table.
    *********************Changes made to document by TRANG TRAN on NOVEMBER 25, 2023*********************
              -- Added 'param' flag to SymTab class variables. This will allow appropriate printing in MIPS code.
*/




#ifndef _SYMTAB
#define _SYMTAB

#include "ast.h"

enum  SYMBOL_SUBTYPE
{
   SYM_SCALAR,
   SYM_FUNCTION,
   SYM_FUNCTION_PROTO,
   SYM_FUNCTION_PRE,
   SYM_ARRAY,
   SYM_UNKNOWN
};

void Display();
int Delete();


struct SymbTab
{
     char *name;
     int offset; /* from activation record boundary */
     int mysize;  /* number of words this item is 1 or more */
     int level;  /* the level where we found the variable */
     enum DataTypes Declared_Type;  /* the type of the symbol */
     enum SYMBOL_SUBTYPE SubType;  /* the subtype of the symbol */
     ASTnode * fparms; /* pointer to parameters of the function in the AST */
     struct SymbTab *next;
     int param; /* a flag for function parameters */
};


struct SymbTab * Insert(char *name, enum DataTypes my_assigned_type , enum  SYMBOL_SUBTYPE sub_type, int  level, int mysize, int offset);

struct SymbTab * Search(char name[], int level, int recur );

static struct SymbTab *first=NULL;   /* global pointers into the symbol table */

char * CreateTemp();

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
int check_params(ASTnode *formalP, ASTnode *actualP);
int proto_func_check(ASTnode *proto, ASTnode *func);
int var_check(ASTnode *p);

int Has_Proto();

#endif

