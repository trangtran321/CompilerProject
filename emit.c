/*
---------------------------------------------------------------------------------------------------------
        Trang Tran
        November 8, 2023

        **Description & Purpose::
            Emitter file for compilers with MIPS code and place it in appropriate open file.
            To translate high level program into assembly, run:
                > make
                > ./compiler < test.al -o test

            This program will generate MIPS assembly code for an incoming 'higher level' program.
            It will print to an .asm file that can be run using the call:
                > java -jar Mars4_5.jar sm <filename.asm>

            - Input:  The AST intermediary language and symbol table from the ast.c and symtable.c files
                      are needed to correctly generate the MIPS code.
            - Output: The MIPS assembly code is generated depending on the rules of each node.

        ***** This program was written between November 8 - November 30, 2023. *****
*/

#include <string.h>
#include "ast.h"
#include "symtable.h"
#include "emit.h"

extern int LTEMP;               //helps keep track of label numbers for while, and if loops

//global variable
int TTEMP = 0;                  //for t-labels in parameters and arguments
int ind = 0;
int stack[50];                  //stack to keep track of while loop labels & print break and continue
int bc_flag = 0;                //flags to determine if break and continue were called by a while-loop or outside of it

//In-file prototypes defined in emit.c and not publicly available
char * CreateT();
void emit_ast(ASTnode * p, FILE * fp);
void emit(FILE *fp, char * label, char* command, char * comment);
void emit_function_dec(ASTnode *p, FILE *fp);
void emit_write(ASTnode *p, FILE *fp);
void emit_expr(ASTnode *p, FILE *fp);
void emit_var(ASTnode *p, FILE *fp);
void emit_read(ASTnode *p, FILE *fp);
void emit_assign(ASTnode *p, FILE *fp);
void emit_select(ASTnode *p, FILE *fp);
void emit_while(ASTnode *p, FILE *fp);
void emit_call(ASTnode *p, FILE *fp);
void emit_params(ASTnode *p, FILE *fp);
int emit_arguments(ASTnode *p, FILE *fp);
void emit_treg(ASTnode *p, FILE *fp);
void emit_return(ASTnode *p, FILE *fp);

/*  * @precondition: p points to the start of a program, fp points to the outfile
    * @postcondition: MIPS code is generated for program
    * @param: ASTnode *p & FILE *fp
    * @description: This function will print the entire program in MIPS - called externally to
    *               translate the algol parsed code into MIPS assembly code.
*/
void EMIT(ASTnode *p, FILE *fp){
        fprintf(fp, "#  Compilers MIPS code Fall 2023\n");
        fprintf(fp, ".data      \n\n");
                emitStrings(p, fp);
        fprintf(fp, "\n");
        fprintf(fp, ".align 2     \n\n");
                emitGlobals(p, fp);
        fprintf(fp, "\n");
        fprintf(fp, ".text      \n\n");
        fprintf(fp, ".globl main      \n\n");
                emit_ast(p, fp);
}//end EMIT

/*  * @precondition: fp points to output file
    * @postcondition: MIPS code is generated
    * @param: FILE *fp, char* label, char* command, char* comment
    * @description: This function prints each line of MIPS code depending on what the
    *               is inputted into the parameters
*/
void emit(FILE *fp, char * label, char* command, char * comment){
        if (strcmp("", comment) == 0){
                if (strcmp("", label) == 0){
                        fprintf(fp, "\t%s\t\t\n", command);
                }
                else{
                        fprintf(fp,"%s:\t%s\t\t\n", label, command);
                }
        }//endif
        else {
                if (strcmp("", label) == 0){
                        fprintf(fp, "\t%s\t\t# %s\n", command, comment);
                }
                else{
                        fprintf(fp,"%s:\t%s\t\t# %s\n", label, command, comment);
                }
        }//endelse
}//end emit

/*  * @precondition: p points to an A_VARDEC node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will print all global variables
*/
void emitGlobals(ASTnode *p, FILE *fp){
        if (p != NULL){
                if(p->nodetype == A_VARDEC && p->symbol->level == 0){
                        int size;
                        size = p->symbol->mysize * word_size;
                        fprintf(fp, "%s: .space   %d\t\t#global variable\n", p->name, size);
                }//endif
                emitGlobals(p->s1, fp);
                emitGlobals(p->s2, fp);
        }//endif
}//end emit_globals

/*  * @precondition: p points to an A_WRITE node, fp points to the outfile
    * @postcondition:
    * @param: ASTnode *p & FILE *fp
    * @description: This function will print all static strings from program to MIPS file under .data
    *               section.
*/
void emitStrings(ASTnode *p, FILE *fp){
        if (p != NULL){
                if (p->nodetype == A_WRITE && p->name != NULL){
                        fprintf(fp, "%s: .asciiz        %s\n", p->nodeLabel, p->name);
                        ind = ind + 1;
                }//endif
                emitStrings(p->s1, fp);
                emitStrings(p->s2, fp);
        }//endif
}//end emitStrings

/*  * @precondition: p points to a node in the AST, fp points to the outfile
    * @postcondition: MIPS code is generated
    * @param: ASTnode *p & FILE *fp
    * @description: This function is the main driver for traversing the abstract symbol tree
    *               to produce MIPS code in the asm file.
*/
void emit_ast(ASTnode * p, FILE * fp){
        char s[100];
        if (p==NULL) return;

        switch (p->nodetype){
                case A_DEC_LIST:        emit_ast(p->s1, fp);
                                        emit_ast(p->s2, fp);
                                        break;

                case A_VARDEC:          emit_ast(p->s1, fp);
                                        emit_ast(p->s2, fp);
                                        break;

                case A_PROTODEC:        break;
                case A_FUNDEC:          emit_function_dec(p, fp);
                                        break;

                case A_COMPOUND:        emit_ast(p->s2, fp);
                                        break;

                case A_EXPRSTMT:        emit_expr(p->s1, fp);
                                        break;

                case A_STMTLIST:        emit_ast(p->s1, fp);
                                        emit_ast(p->s2, fp);
                                        break;

                case A_READ:            emit_read(p, fp);
                                        break;

                case A_WRITE:           emit_write(p, fp);
                                        break;

                case A_ASSIGN:          emit_assign(p, fp);
                                        break;

                case A_SELECTSTMT:      emit_select(p, fp);
                                        break;

                case A_ITERSTMT:        emit_while(p, fp);
                                        break;

                case A_RETURN:          emit_return(p, fp);
                                        break;

                case A_BREAK:           if(bc_flag == 0){
                                                printf("Error:: You can only use a break statement in a while loop.\n");
                                                exit(1);
                                        }
                                        sprintf(s, "j _L%d", stack[ind] - 1);
                                        emit(fp, "", s, "Break:: Jump to end of while.");
                                        break;

                case A_CONT:            if(bc_flag == 0){
                                                printf("Error:: You can only use a continue statement in a while loop.\n");
                                                exit(1);
                                        }
                                        sprintf(s, "j _L%d", stack[ind] - 2);
                                        emit(fp, "", s, "Continue:: Jump to start of while.");
                                        break;

                default:                printf("emit_ast unknown nodetype %d\n", p->nodetype);
                                        printf("Exiting program now. ");
                                        exit(1);
        } //end switch nodetype
}//end emit_ast

/*  * @precondition: p points to a FUNDEC node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will print MIPS code for a function declaration to asm file.
*/
void emit_function_dec(ASTnode *p, FILE *fp){
        char s[100];

        //print function label
        emit(fp, p->name, "", "START OF FUNCTION");
        fprintf(fp, "\n");
        sprintf(s, "subu $a0, $sp, %d", p->symbol->offset * word_size);
        emit(fp, "", s, "Adjust the stack for function setup");
        emit(fp, "", "sw $sp, ($a0)\t", "Store old SP");
        emit(fp, "", "sw $ra, 4($a0)\t", "Store current return address (RA)");
        emit(fp, "", "move $sp, $a0\t", "Adjust SP");
        fprintf(fp, "\n");

        //check to make sure there are no more than 8 parameters required
        emit_params(p->s1, fp);

        //prints for compound statements
        emit_ast(p->s2, fp);

        //function will return to the original address before it was called
        fprintf(fp, "\n");
        emit(fp, "", "li $a0, 0\t\t", "RETURN has no specified value set to 0");
        emit(fp, "", "lw $ra, 4($sp)\t", "Restore RA");
        emit(fp, "", "lw $sp, ($sp)\t", "Restore SP");

        //if this is main function, we will exit entire program
        if (strcmp(p->name, "main") == 0){
                emit(fp, "", "li $v0, 10\t\t", "Leave main program");
                emit(fp, "", "syscall\t\t\t", "Exiting entire program");
        }//endif
        else{
                emit(fp, "", "jr $ra\t\t\t", "Return to caller");
                fprintf(fp, "\n\n");
        }//endelse
}//end emit_function_dec

/*  * @precondition: p points to the start of a program, fp points to the outfile
    * @postcondition: returns the number of parameters that are required in the function.
    * @param: ASTnode *p & FILE *fp
    * @description: This recursive function creates a temporary label for formal parameters
    *               of a function, prints the MIPS code that corresponds with loading a
    *               temporary variable into the function, and keeps count of how many parameters
    *               have been asked for by the input program.
*/
void emit_params(ASTnode* p, FILE *fp){
        char* tlabel = CreateT();
        char s[100];

        if (p->symbol != NULL){
                sprintf(s, "sw %s, %d($sp)\t", tlabel, p->symbol->offset * word_size);
                emit(fp, "", s, "Load temp variable int formal parameter");

                //if there are more parameters to print, print them.
                if (p->s2 != NULL){
                        emit_params(p->s2, fp);
                }//endif
        }//endif

        //reset TTEMP as these temporary labels are only for the inside of each function.
        TTEMP = 0;

}//end emit_params

/*  * @precondition: p points to an A_WRITE node, fp points to the outfile
    * @postcondition: MIPS code will be generated
    * @param: ASTnode *p & FILE *fp
    * @description: This function differentiates between strings and expressions and
    *               will write generate MIPS code that prints to terminal.
*/
void emit_write(ASTnode *p, FILE *fp){
        char s[100];

        //write string
        if (p->name != NULL){
                sprintf(s, "la $a0, %s\t\t", p->nodeLabel);
                emit(fp, "", "li $v0, 4\t\t", "Print a string");
                emit(fp, "", s, "Print a string");
                emit(fp, "", "syscall\t\t\t", "Perform write");
                fprintf(fp, "\n");
        }//endif

        //write expression
        else{   emit_expr(p->s1, fp);
                emit(fp, "", "li $v0 1\t\t", "Print number");
                emit(fp, "", "syscall\t\t\t", "Perform write");
                fprintf(fp, "\n");
        }//endelse
}//end emit_write


/*  * @precondition: p points to an expression tree component, fp points to the outfile
    * @postcondition:
    * @param: ASTnode *p & FILE *fp
    * @description: This function will generate MIPS code for an entire expression statement
    *               tree. This encompasses nodes A_EXPR, A_NUM, A_VAR and A_CALL. $a0 will
    *               hold the value set by the generated MIPS code after each expression evaluation.
*/
void emit_expr(ASTnode *p, FILE *fp){
        char s[100];

        if (p == NULL) {
                printf("Illegal use of emit_expr with null pointer.\n");
                exit(1);
        }//endif

        //deal with base cases
        switch (p->nodetype) {
                case A_FACTOR:
                case A_NUM:     sprintf(s, "li $a0 %d\t\t", p->value);
                                emit(fp, "", s, "Expression is a constant");
                                return;
                                break;

                case A_ARGLIST:
                case A_VAR:     emit_var(p, fp);
                                if(p->value == 0){
                                        emit(fp, "", "lw $a0, ($a0)\t", "Expression is A_VAR, get value.");
                                }
                                return;
                                break;

                case A_CALL:    emit_call(p,fp);
                                break;

                case A_EXPR:
                        switch (p->operator){
                                case A_PLUS:    emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - PLUS:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - PLUS:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - PLUS:: restore LHS from memory");
                                                emit(fp,"", "add $a0, $a0, $a1", "Expr - PLUS:: add $a0 and $a1, placing back into $a0");
                                                fprintf(fp, "\n");
                                                break;

                                case A_MINUS:   emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - MINUS:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - MINUS:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - MINUS:: restore LHS from memory");
                                                emit(fp,"", "sub $a0, $a0, $a1", "Expr - MINUS:: subtract $a1 from $a0, placing back into $a0");
                                                fprintf(fp, "\n");
                                                break;

                                case A_MUL:     emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - MUL:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - MUL:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - MUL:: restore LHS from memory");
                                                emit(fp,"", "mult $a0, $a1\t", "Expr - MUL:: perform multiplication on $a0 and $a1");
                                                emit(fp, "", "mflo $a0\t\t", "Expr - MUL:: output and store answer in $a0");
                                                fprintf(fp, "\n");
                                                break;

                                case A_DIV:     emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - DIV:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - DIV:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - DIV:: restore LHS from memory");
                                                emit(fp,"", "div $a0, $a1\t", "Expr - DIV:: perform multiplication on $a0 and $a1");
                                                emit(fp, "", "mflo $a0\t\t", "Expr - DIV:: output and store answer in $a0");
                                                fprintf(fp, "\n");
                                                break;

                                case A_OR:      emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - OR:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - OR:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - OR:: restore LHS from memory");
                                                emit(fp, "", "or $a0, $a0, $a1\t", "Expr - OR:: check equality");
                                                break;

                                case A_AND:     emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - AND:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - AND:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - AND:: restore LHS from memory");
                                                emit(fp, "", "and $a0, $a0 $a1", "Expr - AND:: store output in $a0");
                                                break;

                                case A_LE:      emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - LE:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - LE:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - LE:: restore LHS from memory");
                                                emit(fp,"", "slt $a0, $a0, $a1", "Expr - LE:: compare and save value in $a0");
                                                fprintf(fp, "\n");
                                                break;

                                case A_GR:      emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - GR:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - GR:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - GR:: restore LHS from memory");
                                                emit(fp,"", "slt $a0, $a1, $a0", "Expr - GR:: compare and save value in $a0");
                                                fprintf(fp, "\n");
                                                break;

                                case A_LEEQU:   emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - LEEQU:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - LEEQU:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - LEEQU:: restore LHS from memory");
                                                emit(fp, "", "add $a1, $a1, 1\t", "Expr - LEEQU:: add one to do compare");
                                                emit(fp,"", "slt $a0, $a0, $a1", "Expr - LEEQU:: compare and save value in $a0");
                                                fprintf(fp, "\n");
                                                break;

                                case A_GREQU:   emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - GREQU:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - GREQU:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - GREQU:: restore LHS from memory");
                                                emit(fp, "", "add $a0, $a0, 1\t", "Expr - GREQU:: add one to do compare");
                                                emit(fp,"", "slt $a0, $a1, $a0", "Expr - GREQU:: compare and save value in $a0");
                                                fprintf(fp, "\n");
                                                break;

                                case A_EQUAL:   emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - EQUAL:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - EQUAL:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - GREQU:: restore LHS from memory");
                                                emit(fp, "", "slt $t2, $a0, $a1", "Expr - EQUAL:: place the greater than value in $t2");
                                                emit(fp,"", "slt $t3, $a1, $a0", "Expr - EQUAL:: compare and save value in $a0");
                                                emit(fp, "", "nor $a0, $t2, $t3", "Expr - EQUAL:: check equality");
                                                emit(fp, "", "andi $a0, 1\t\t", "Expr - EQUAL:: store output in $a0");
                                                fprintf(fp, "\n");
                                                break;

                                case A_NEQU:    emit_expr(p->s1, fp);
                                                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - NEQU:: stores LHS temporarily");
                                                emit_expr(p->s2, fp);
                                                emit(fp, "", "move $a1, $a0\t", "Expr - NEQU:: right hand side needs to be a1");
                                                sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
                                                emit(fp, "", s, "Expr - GREQU:: restore LHS from memory");
                                                emit(fp, "", "slt $t2, $a0, $a1", "Expr - NEQU:: place the greater than value in $t2");
                                                emit(fp,"", "slt $t3, $a1, $a0", "Expr - NEQU:: compare and save value in $a0");
                                                emit(fp, "", "or $a0, $t2, $t3", "Expr - NEQU:: check equality");
                                                fprintf(fp, "\n");
                                                break;

                                case A_NOT:     emit_expr(p->s1, fp);
                                                emit(fp, "", "xori $a0 $a0 1", "Expr - NOT:: check equality");
                                                break;

                                default:        printf("emit_expr operator not known. \n");
                                                exit(1);
                        }
                        return;
                        break;
                default:        printf("emit_expr base case not known. \n");
                                printf("%d nodetype\n", p->nodetype);
                                exit(1);
        }//end of switch
}//end emit_expr

//pointer to A_VARs
//$a0 will the the memory location of the variable
/*  * @precondition: p points to an A_VAR node, fp points to the outfile
    * @postcondition:
    * @param: ASTnode *p & FILE *fp
    * @description: This function will generate MIPS code for a variable. It differentiates
    *               between global, local, scalar and array variables and will print appropriate
    *               MIPS code to handle each type.
*/
void emit_var(ASTnode *p, FILE *fp){
        char s[100];

        //if A_VAR is a global variable
        if (p->symbol->level == 0){
                if (p->symbol->SubType == SYM_ARRAY && p->s1 != NULL){
                        emit_expr(p->s1, fp);
                        emit(fp, "", "move $a1, $a0\t", "Emit VAR:: Copying array index into $a1");
                        emit(fp, "", "sll $a1, $a1 2\t", "Multiply the index by wordsize via SLL");
                        sprintf(s, "la $a0, %s\t\t", p->name);
                        emit(fp, "", s, "\tEmit VAR:: global variable");
                        emit(fp, "", "add $a0 $a0 $a1\t", "Emit VAR:: array plus internal offset");
                        emit(fp, "", "lw $a0, ($a0)\t\t", "Expression is A_VAR, get value.");
                }//end if-array
                else{
                        sprintf(s, "la $a0, %s", p->name);
                        emit(fp, "", s, "\tEmit VAR:: global variable");
                }//end else
        }//endif

        else{
                if (p->symbol->SubType == SYM_ARRAY && p->s1 != NULL){
                        emit_expr(p->s1, fp);
                        emit(fp, "", "move $a1, $a0\t", "Emit VAR:: Copying array index into $a1");
                        emit(fp, "", "sll $a1, $a1 2\t", "Multiply the index by wordsize via SLL");
                        emit(fp, "", "move $a0, $sp\t", "Emit VAR:: local variable found. Make copy of SP.");
                        sprintf(s, "addi $a0, $a0 %d\t", (p->symbol->offset) * word_size);
                        emit(fp, "", s, "Emit VAR:: local stack pointer plus offset to get location of local variable");
                        if (p->symbol->param == 1){
                                emit(fp, "", "lw $a0, ($a0)\t\t", "Get address of function array parameter");
                        }//endif
                        emit(fp, "", "add $a0 $a0 $a1\t", "Emit VAR:: array plus internal offset");
                }//end if-array

                else{
                        emit(fp, "", "move $a0, $sp\t", "Emit VAR:: local variable found. Make copy of SP");
                        sprintf(s, "addi $a0, $a0 %d\t", (p->symbol->offset) * word_size);
                        emit(fp, "", s, "Emit VAR:: local stack pointer plus offset to get location of local variable");
                }//end else
        }//endelse
}//end emit_var

/*  * @precondition: p points to an A_READ node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will generate MIPS code to generate location of A_VAR, read
    *               in from user input, and store it in the memory location of A_VAR.
*/
void emit_read(ASTnode *p, FILE *fp){
        emit_var(p->s1, fp);    //$a0 is memory location
        emit(fp, "", "li $v0, 5\t\t", "Read number from input");
        emit(fp, "", "syscall\t\t\t", "Reading number");
        emit(fp, "", "sw $v0, ($a0)\t", "Store read input into memory location");
        fprintf(fp, "\n");
}//end emit_read


/*  * @precondition: p points to an A_ASSIGN node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will generate MIPS code to call operator, and assign the
    *               result into the LHS operand's memory location.
*/
void emit_assign(ASTnode *p, FILE *fp){
        char s[100];

        emit_expr(p->s2, fp);
        sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
        emit(fp, "", s, "Assign:: Stores RHS temporarily");
        emit_var(p->s1, fp);
        sprintf(s, "lw $a1, %d($sp)\t", p->symbol->offset * word_size);
        emit(fp, "", s, "Assign:: Gets RHS temporarily");
        emit(fp, "", "sw $a1, ($a0)\t", "Assign:: Places RHS into memory");
        fprintf(fp, "\n");
}//end emit_assign


/*  * @precondition: p points to an A_SELECTSTMT node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will generate MIPS code to generate labels for different jump
    *               destinations for if and else statements.
*/
void emit_select(ASTnode *p, FILE *fp){
        char* label1;
        char* label2;
        char s[100];

        //print if-statement condition
        emit_expr(p->s1, fp);
        fprintf(fp, "\t\t\t\t\t#IF BODY \n");

        //label to else branch if condition in p->s1 is not met
        label1 = CreateLabel();
        label2 = CreateLabel();

        //increment index and push current label onto stack
        ind = ind + 1;
        stack[ind] = LTEMP;

        sprintf(s,"beq $a0 $0, %s\t", label1);
        emit(fp, "", s, "If:: branch to else-body");
        emit_ast(p->s2->s1, fp);

        //label to end of if-else statement
        sprintf(s, "j %s\t\t\t", label2);
        emit(fp, "", s, "If:: if-body end");

        //print else body
        fprintf(fp, "%s:\t\t\t\t#ELSE:: label\n", label1);
        fprintf(fp, "\t\t\t\t\t#ELSE BODY\n");
        emit_ast(p->s2->s2, fp);

        //label to end of else-statement
        fprintf(fp, "%s:\t\t\t\t#END OF IF:: label\n", label2);

        //pop current label off stack and decrement index by one
        stack[ind] = 0;
        ind = ind - 1;
}//end_emit_select

//Pre: a pointer to while node
//Post: MIPS code to generate a while loop
/*  * @precondition: p points to an A_ITERSTMT node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will generate MIPS code and labels for the implementation of a while loop.
*/
void emit_while(ASTnode *p, FILE *fp){
        char* label1;
        char* label2;
        char s[100];
        //set flag to true
        bc_flag = 1;

        //create label for begining of while loop, increment global variable, ind, to
        //reflect increase in labels
        label1 = CreateLabel();
        label2 = CreateLabel();

        //increase index and push current label onto stack
        ind = ind + 1;
        stack[ind] = LTEMP;

        fprintf(fp, "\n");
        emit(fp, label1, "", "While:: Start of while loop label");
        emit_expr(p->s1, fp);
        sprintf(s, "beq $a0 $0 %s", label2);
        emit(fp, "",s, "While:: Branch to end of loop if condition is met.");

        //print compound statement
        emit_ast(p->s2, fp);

        //print jump back to top of loop
        sprintf(s, "j %s", label1);
        emit(fp, "", s, "While:: Jump back to begining of while loop.");
        fprintf(fp, "\n");

        //label that signifies end of while loop
        emit(fp, label2, "", "While:: End of loop.");

        //Pop current label off stack and decrement index
        stack[ind] = 0;
        ind = ind - 1;

        //reset flags
        bc_flag = 0;
}//end emit_while

/*  * @precondition: none
    * @postcondition: generates temporary register labels
*/
char * CreateT()
{    char hold[100];
     char *s;
     sprintf(hold,"$t%d",TTEMP++);
     s=strdup(hold);
     return (s);
}//end CreateT

/*  * @precondition: p points to an A_CALL node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will generate MIPS code to call a function.
*/
void emit_call(ASTnode *p, FILE *fp){
        int num;
        char s[100];

        fprintf(fp, "\n");
        fprintf(fp, "\t\t\t#Setting Up Function Call\n");
        fprintf(fp, "\t\t\t#Evaluate Function Parameters\n");
        if (p->s1 != NULL){
                //emit_arguments
                num = emit_arguments(p->s1, fp);
                if (num > 8){
                        printf("ERROR: Cannot have more than 8 arguments in a function call.");
                        exit(1);
                }//endif

                //emit temporary registers
                emit_treg(p->s1, fp);
        }//endif

        sprintf(s, "jal %s\t", p->name);
        emit(fp, "", s, "Call the function");
        fprintf(fp, "\n");
}//end emit_call

//pre: pointer to A_ARGS
//post: generates MIPS code for getting all actual arguments in a function call
//and place them in temporary variables
/*  * @precondition: p points to an A_ARGS node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will generate MIPS code get the actual arguments, get their
    *               memory location and store them temporarily in $a0
*/
int emit_arguments(ASTnode *p, FILE *fp){
        char s[100];

        if (p != NULL){;
                emit_expr(p->s1, fp);
                sprintf(s, "sw $a0, %d($sp)\t", p->symbol->offset * word_size);
                emit(fp, "", s, "Call ARG:: store argument temporarily");
                if (p->s2 != NULL){
                        return emit_arguments(p->s2, fp) + 1;
                }//endif
                return 1;
        }//endif
        return 0;
}//end emit_arguments


/*  * @precondition: p points to an A_ARGS node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will generate MIPS code get the stored memory location
    *               and move into temporary register so that the function can work with the
    *               location
*/
void emit_treg(ASTnode *p, FILE *fp){
        char s[100];
        char* tlabel = CreateT();

        sprintf(s, "lw $a0, %d($sp)\t", p->symbol->offset * word_size);
        emit(fp, "", s, "ArgList:: Pull out stored argument and place in $a0");
        sprintf(s, "move %s, $a0\t", tlabel);
        emit(fp, "", s, "ArgList:: Move argument from $a0 to temporary register");

        if (p->s2 != NULL){
                emit_treg(p->s2, fp);
        }//endif

        TTEMP = 0;

}// end emit_treg


/*  * @precondition: p points to an A_RETURN node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will generate MIPS code to restore the old register address,
    *               stack pointer and return to the calling function's register address.
*/
void emit_return(ASTnode *p, FILE *fp){
        if (p->s1 != NULL){
                emit_expr(p->s1, fp);
        }
        else {
                emit(fp, "", "li $a0, 0", "Return:: has no specified value set to 0");
        }
        emit(fp, "", "lw $ra 4($sp)\t", "Return:: Restore old RA");
        emit(fp, "", "lw $sp ($sp)\t", "Return:: Return from function and store SP");
        emit(fp, "", "jr $ra\t\t\t", "Return:: return to caller");
}//end emit_return
