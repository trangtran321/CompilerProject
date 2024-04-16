/*    Abstract syntax tree code
      Header file
      Shaun Cooper January 2022
   ******************************************************************************************************************
            Trang Tran
            September 27, 2023
            Lab 6 : ast.h

            **Description & Purpose::
              - This header file declares the ASTprint, ASTcreateNode and PT functions.
              - This header file defines the ASTnodeType struct.
              - This header file declares the enumerated types ASTNode, DataTypes and OPERATORS.
              - Lastly, this header file instantiates a global ASTnode *program that serves as a pointer
                to the abstract symbol tree's root.

   ************************* CHANGES MADE TO DOCUMENT SEPTEMBER 27 - TRANG TRAN *************************
            --  Added ASTNode Types to enum ASTtype that match the production rules stated in yacc file.
            --  Added OPERATORS to include relative operators, not, true, false, and multiplicative operators.
            --  Added DataTypes to include void, boolean, and Strings for variable and function declarations.
      ************************* CHANGES MADE TO DOCUMENT SEPTEMBER 27 - TRANG TRAN *************************
            --  Deleted A_PARAMS, A_ARGS, and A_ARGLIST
            --  Added 'symbol', a struct SymbTab pointer class variable to ASTNode.
*/

/*
   The following statement imports all external libraries into our files and makes them available to
   both the ast.c and yacc file.
*/
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include "symtable.h"

#ifndef AST_H
#define AST_H
extern int mydebug;           //imported from LEX file - if set to 1, will print all debug statements across files.

/*
   @description:  ASTtype are enumerated to represent the production rule that was triggered in yacc by specific
                  tokens that were sent via LEX. They are names of nodes in the abstract symbol tree.
*/
enum ASTtype {
   A_PROGRAM,              //0
   A_DEC_LIST,             //1
   A_VARDEC,               //2
   A_FUNDEC,               //3
   A_PROTODEC,             //4
   A_PARAMS,               //5
   A_PARAM,                //6
   A_STMTLIST,             //7
   A_LOCALDEC,             //8
   A_COMPOUND,             //9
   A_EXPRSTMT,             //10
   A_SELECTSTMT,           //11
   A_ASSIGNSTMT,           //12
   A_WRITE,                //13
   A_IDENT,                //14
   A_EXPR,                 //15
   A_VAR,                  //16
   A_TERM,                 //17
   A_FACTOR,               //18
   A_NUM,                  //19
   A_IFBODY,               //20
   A_ITERSTMT,             //21
   A_RETURN,               //22
   A_READ,                 //23
   A_ASSIGN,               //24
   A_BREAK,                //25
   A_CALL,                 //26
   A_ARGLIST,              //27
   A_CONT                  //28
};

/*
   @description:  The enumeration of DataTypes represents data types that are associated with variable declarations
                  or functions.
*/
enum DataTypes {
   A_INTTYPE,
   A_VOIDTYPE,
   A_BOOLEANTYPE,
   A_UNKNOWN
};

/*
   @description:  The enumeration of OPERATORS includes additive, multiplicative, and relative operators as well as
                  'true', 'false' and 'not'.
*/
enum OPERATORS {
   A_PLUS,
   A_MINUS,
   A_MUL,
   A_DIV,
   A_OR,
   A_AND,
   A_LE,
   A_GR,
   A_LEEQU,
   A_GREQU,
   A_NEQU,
   A_EQUAL,
   A_NOT
};

/*
   @description:  The ASTnodetype struct holds pointers to other instances of the struct
                  as well as the properties of the struct which include the nodetype, operator,
                  datatype, name and value. The pointers allow us to represent the parsed code in
                  an abstract symbol tree.
*/
typedef struct ASTnodetype
{
     enum ASTtype nodetype;            //This is the name of the production rule
     enum OPERATORS operator;          //if the production rule has an operator, put it here
     enum DataTypes datatype;          //Is there a type that's associated with this ?
     char * name;                      //This is the variable name or function name ...etc
     int value;                        //The value depends on the production rule. (i.e. in int x [100] - value will be 100, but also x = 100 is a AssignRule where value is 100)
     struct ASTnodetype *s1,*s2 ; /* used for holding IF and WHILE components -- not very descriptive */
     struct SymbTab *symbol;
     char * nodeLabel;
} ASTnode;


/*
   @precondition: The ASTtype has been declared in ast.h and the yacc file recognizes the datatype in the %union
                  function.
   @postcondition: ASTnode* is declared
   @param: enum ASTtype
	@return: ASTnode*
   @description:  This function will create an ASTnode pointer of a specific ASTtype. It uses malloc to
                  create an ASTnode and passes back the heap address of the newly created node.
*/
ASTnode *ASTCreateNode(enum ASTtype mytype);

/*
   @precondition:A number representing how many spaces to print is inputted
   @postcondition: Prints the appropriate number of spaces to terminal
   @param: int howmany
	@return: None
   @description:  This function will print spaces depending on the level of the parse in the abstract symbol tree.
*/
void PT(int howmany);


/*
   @precondition: The tree has been declared and there is at least one node in it.
   @postcondition: None
   @param: int level, ASTnode *p
	@return: None
   @description:  This function will print the abstract symbol tree that represents the inputted program.
*/
void ASTprint(int level,ASTnode *p);

/*
   program is a global variable that is a pointer to the root of the abstract symbol tree. It is used at the start
   of the production rules in the yacc file and to point ASTprint() in the right direction.
*/
ASTnode *program; // pointer to the tree

char * DataTypeToString(enum DataTypes mydatatype);

char * CreateLabel();

#endif // of AST_H
