/*      Trang Tran
        October 1, 2023

        **Description & Purpose::
          --  The ast.c program allows us to create ASTnodes and prints the abstract symbol tree that
              represents an inputted program
          --  The function ASTCreateNode will allow us to create a pointer to a node on the abstract
              symbol tree. The pointer's intitial values are set to NULL and 0 - all of which are set
              in the yacc file.
          --  The ASTprint function is what prints the abstract symbol tree. It uses a switch statement
              that matches the nodetype to how the printing should be formatted. If the s1 and s2 pointers
              are not NULL, ASTprint will recursively go to the ASTnode that s1 or s2 point to and print
              them as well. This print function performs prefix printing, which is where the operator
              is in front and the values follow.
          -- The auxiliary functions PT() and DataTypeToString() asist in either printing tabs to keep the
             format of a tree or to print the datatypes associated with nodes.

            -- Input:   The yacc file uses the functions defined in this file but also provides input in terms
                        of the nodetypes, datatypes, names, values and properties associated to nodes via the
                        tokens it has parsed.
            -- Output:  An abstract symbol tree is printed to the terminal.
    ****************************CHANGES MADE TO DOCUMENT OCTOBER 7, 2023 - TRANG TRAN ***********************************
                    -- Added cases to switch statement in ASTprint() to ensure they match with production rules in lab6.y file.
                    -- Made sure all cases were properly formatted when printed to the terminal.
                    -- Added datatypes to DataTypeToString() to ensure 'void', 'int' and 'boolean' datatypes can be printed
                        properly.
                    -- Wrote PT() function.
    ****************************CHANGES MADE TO DOCUMENT OCTOBER 20, 2023 - TRANG TRAN ***********************************
                    -- Deleted cases A_ARGS, A_ARGLIST, and A_PARAMLIST because they are no longer their own nodes
                       but Params and Var are their own lists.
                    -- Added offset printing to the output for each variable declaration, formal and actual parameter
                       places in the symbol table.
*/
#include <string.h>
#include<stdio.h>
#include <stdlib.h>
#include<malloc.h>
#include "ast.h"
#include "symtable.h"

int LTEMP=0;  /* Global Label Temp counter */
/*
    @precondition:  The ASTtype has been declared in ast.h and the yacc file recognizes the datatype in the %union
                    function.
    @postcondition: An ASTnode is declared
    @param: ASTtype
    @return: ASTnode*
    @description:   This function will create an ASTnode pointer of a specific ASTtype. It uses malloc to
                    create an ASTnode and passes back the heap address of the newly created node.
*/
ASTnode *ASTCreateNode(enum ASTtype mytype)
{
    ASTnode *p;
    if (mydebug) fprintf(stderr,"Creating AST Node \n");
    p=(ASTnode *)malloc(sizeof(ASTnode));
    p->nodetype=mytype;
    p->s1=NULL;
    p->s2=NULL;
    p->value=0;
    return(p);
}

/*
    @precondition: A number representing how many spaces to print is inputted
    @postcondition: Prints the appropriate number of spaces to terminal
    @param: int howmany
	@return: None
    @description:  This function will print spaces depending on the level of the parse in the abstract symbol tree.
*/
void PT(int howmany)
{
	while (howmany > 0){
        printf("  ");
        howmany--;
    } //end of while
}

/*
    @precondition: A datatype is inputted
    @postcondition: None
    @param: enum DataTypes
	@return: A char* string that represents the datatype entered
    @description:  This auxiliary function will return a string of the datatype entered
*/

char * DataTypeToString(enum DataTypes mydatatype){
    switch (mydatatype) {
            case A_VOIDTYPE:    return ("void");
                                break;
            case A_INTTYPE:     return("int");
                                break;
            case A_BOOLEANTYPE: return("boolean");
                                break;
            default: printf("Unknown type in DataTypeToString\n");
                     exit(1);
      } //of switch
}// of DataTypeToString()


/*
    @precondition: The tree has been declared and there is at least one node in it.
                    All the ASTnodetypes must be declared in ast.h file and properly linked
                    to a production rule in the yacc file in order to be printed.
    @postcondition: None
    @param: int level, ASTnode *p
	@return: None
    @description:  The ASTprint function is what prints the abstract symbol tree. It uses a switch statement
              that matches the nodetype to how the printing should be formatted. If the s1 and s2 pointers
              are not NULL, ASTprint will recursively go to the ASTnode that s1 or s2 point to and print
              them as well. This print function performs prefix printing, which is where the operator
              is in front and the values follow.
*/
void ASTprint(int level, ASTnode *p)
{
   int i;
   if (p == NULL ) return;

    /* When p is not NULL, the switch statement will find the corresponding nodetype
        and print what is appropriate for that nodetype. If pointers s1 and s2 are not null,
        the function is recursively called to print the nodes that are pointed to.
    */
   switch (p->nodetype) {
        case A_DEC_LIST:    ASTprint(level, p->s1);             //points to a declaration (variable, function or prototype)
                            ASTprint(level, p->s2);             //points to the rest of the declarations
                            break;

        case A_VARDEC:      PT(level);
                            printf("VARIABLE ");
                            printf("%s ", DataTypeToString(p->datatype));
                            printf(" %s",p->name);              //variable name
                            if (p->value > 0)
                                printf("[%d]",p->value);        //if variable is an array, print the array length
                            PT(level);
                            printf(" with offset %d and level %d\n", p->symbol->offset,  p -> symbol -> level);
                            printf("\n");
		                    ASTprint(level,p->s1);              //if there is a variable list, print next variable
                            break;

        case A_FUNDEC :     PT(level);
                            printf("FUNCTION ");
                            printf("%s ", DataTypeToString(p->datatype));
                            printf("%s ",p->name);              //function name
                            printf(" with offset %d\n",p->symbol->offset);
                            printf("(\n");
		                    ASTprint(level+1,p->s1);            //parameters
                            printf(")");
                            ASTprint(level+1,p->s2);           //compound statements
                            printf("\n");
                            break;

        case A_PROTODEC :   PT(level);
                            printf("FUNCTION PROTOTYPE ");
                            printf("%s ", DataTypeToString(p->datatype));
                            printf("%s ",p->name);              //function name
                            printf(" with offset %d\n",p->symbol->offset);
                            printf("(\n");
		                    ASTprint(level,p->s1);              //parameters
                            printf(")\n");
                            break;

        case A_PARAMS:      PT(level);
                            if (p->datatype == A_VOIDTYPE){
                                printf("VOID\n");                 //void parameter
                            } //end of if
                            else {
                                PT(level);
                                printf("PARAMETER LIST: \n");
                            } // end of else
                            break;

        case A_PARAM:       PT(level);
                            printf("PARAMETER ");
                            printf("%s ", DataTypeToString(p->datatype));   //parameter datatype
                            printf("%s ", p-> name);                        //parameter name
                            if (p->value == -1){
                                printf("[]");
                            }
                            printf(" \t");
                            printf(" with offset %d and level %d\n", p->symbol->offset,  p -> symbol -> level);
                            ASTprint(level, p->s2);
                            break;

        case A_LOCALDEC:    ASTprint(level, p->s1);             //points to a variable declaration
                            ASTprint(level, p->s2);             //points to the rest of the local declarations
                            break;

        case A_STMTLIST:    ASTprint(level, p->s1);             //points to a statement
                            ASTprint(level, p->s2);             //points to the rest of the statements
                            break;

        case A_COMPOUND:    PT(level);
                            printf("BEGIN \n");
                            ASTprint(level+1, p->s1);           //points to a local declaration
                            ASTprint(level+1, p->s2);           //points to a statement list
                            PT(level);
                            printf("END\n");
                            break;

        case A_EXPRSTMT:    PT(level);
                            printf("EXPRESSION STATEMENT\n");
                            ASTprint(level, p->s1);             //points to an Expression
                            break;

        case A_SELECTSTMT:  PT(level);
                            printf("IF \n");
                            ASTprint(level+1, p->s1);           //points to an if condition statement
                            PT(level);
                            printf("IF Body \n");
                            ASTprint(level+1, p->s2);           //points to the if-body node
                            break;

        case A_IFBODY:      ASTprint(level, p->s1);             //points to the then-statement
                            if(p->s2 !=NULL){
                                PT(level-1);
                                printf("ELSE \n");
                                ASTprint(level, p->s2);         //points to the else-statement/s
                            } //end of if
                            break;

        case A_ASSIGN:      PT(level);
                            printf("ASSIGNMENT\n");
                            PT(level+1);
                            printf("LEFTHAND SIDE\n");
                            ASTprint(level+2, p->s1);           //points to the lefthand side of assignment statement
                            PT(level+1);
                            printf("RIGHTHAND SIDE\n");
                            ASTprint(level+2, p->s2);           //points to the righthand side of the assignment statement
                            break;



        case A_ITERSTMT:    PT(level);
                            printf("WHILE\n");
                            ASTprint(level+1, p->s1);           //points to the while condition
                            PT(level);
                            printf("DO\n");
                            ASTprint(level+1, p->s2);           //points to the do-statement
                            break;

        case A_RETURN:      PT(level);
                            printf("RETURN\n");
                            ASTprint(level, p->s1);             //points to the expression being returned
                            printf("\n");
                            break;


        case A_WRITE :      PT(level);
                            printf("WRITE\n");
                            if (p->name != NULL){
                                PT(level+1);
                                printf("STRING: %s \n", p->name);       //prints the inputed STRING
                            } //end of if
                            else {
                                ASTprint(level+1, p->s1);               //points to an expression to be printed
                            } //end of else
                            break;

        case A_READ:        PT(level);
                            printf("READ: \n");
                            ASTprint(level+1, p->s1);                   //points to the expression being read in
                            break;

        case A_CONT:        PT(level);
                            printf("CONTINUE \n");
                            break;

        case A_BREAK:       PT(level);
                            printf("BREAK \n");
                            break;

        case A_EXPR:        PT(level);
                            //Inner switch statement prints out operators passed from yacc to ASTprint()
                            switch (p->operator) {
                                case A_PLUS:    printf("EXPR  + \n");
                                                break;
                                case A_MINUS:   printf("EXPR  - \n");
                                                break;
                                case A_MUL:     printf("EXPR  *  \n");
                                                break;
                                case A_DIV:     printf("EXPR  /  \n");
                                                break;
                                case A_OR:      printf("EXPR  or \n");
                                                break;
                                case A_AND:     printf("EXPR  and \n");
                                                break;
                                case A_LE:      printf("EXPR  < \n");
                                                break;
                                case A_GR:      printf("EXPR  > \n");
                                                break;
                                case A_LEEQU:   printf("EXPR  <= \n");
                                                break;
                                case A_GREQU:   printf("EXPR  >= \n");
                                                break;
                                case A_NEQU:    printf("EXPR  != \n");
                                                break;
                                case A_EQUAL:   printf("EXPR  == \n");
                                                break;
                                case A_NOT:     PT(level);
                                                printf("EXPR: 'not' \n");
                                                break;
                                default: printf("Unknown operator in A_EXPR ASTprint()");
                                            printf("Exiting A_EXPR now");
                                            exit(1);
                            } //end switch

                            ASTprint(level+1, p->s1);   //points to lefthand side of operation
                            ASTprint(level+1, p->s2);   //points to righthand side of operation
                            break;

        case A_VAR:         PT(level);
                            if (p->symbol->SubType == SYM_ARRAY){
                                printf("VAR with name %s \t", p->name);     //prints variable name
                                PT(level);
                                printf("[\n");
                                ASTprint(level, p->s1);                     //points to expression inside of an array bracket
                                PT(level);
                                printf("] with offset %d and level %d\n", p->symbol->offset,  p -> symbol -> level);
                            } //end of if
                            else {
                                printf("VAR with name %s\t", p->name);     //prints variable name
                                printf(" with offset %d and level %d\n", p->symbol->offset,  p -> symbol -> level);
                            } //end of else
                            break;

        case A_TERM:        PT(level);
                            printf("TERM: \n");
                            if (p->s2 != NULL){
                                ASTprint(level, p->s1);                     //points to Term to be factored
                                ASTprint(level, p->s2);                     //points to Factor
                            } // end of if
                            else {
                                PT(level);
                                ASTprint(level, p->s1);                     //points to Factor
                            } //end of else
                            break;

        case A_FACTOR:      PT(level);
                            if (p->s1 != NULL) ASTprint(level, p->s1);      //points to either an A_NUM node or an A_EXPR node that
                                                                            //represents 'NOT'
                            PT(level+1);
                            if (p->name != NULL) printf("%s\n", p->name);    //prints factor's name
                            break;

        case A_CALL:        PT(level);
                            printf("CALL %s \n", p->name);                  //function name
                            PT(level+1);
                            printf("(\n");
                            ASTprint(level+2, p->s1);                       //points to arguments
                            PT(level+1);
                            printf(") \n");
                            break;

        case A_ARGLIST:     PT(level);
                            printf("CALL ARGUMENT\n");
                            ASTprint(level+1, p->s1);                       //points to an argument inside of a function call
                            ASTprint(level, p->s2);                         //points to the rest of the arguments in a function call
                            break;

        case A_NUM:         PT(level);
                            printf("Number with value %d\n", p->value);     //literal numerical value
                            break;

        default: printf("\nUnknown type in ASTprint %d\n", p->nodetype);
                 printf("Exiting ASTprint immediately\n");
                 exit(1);
       } // end of switch
} // end of ASTprint

char * CreateLabel()
{    char hold[100];
     char *s;
     sprintf(hold,"_L%d",LTEMP++);
     s=strdup(hold);
     return (s);
}

/* dummy main program so I can compile for syntax error independently
main()
{
}
/* */
