/*
---------------------------------------------------------------------------------------------------------
        Trang Tran
        November 8, 2023
        Lab 9 : emit.h

        **Description & Purpose::
            This header file connects the abstract symbol tree with the emitter file.
*/

#ifndef EMIT_H
#define EMIT_H

#include "ast.h"

#define word_size 4     //global variable to multiply offsets with to set aside correct amount of memory
#define log_wsize 2     //used in 'sll' calls if needed

/*  * @precondition: p points to the start of a program, fp points to the outfile
    * @postcondition: MIPS code is generated for program
    * @param: ASTnode *p & FILE *fp
    * @description: This function will print the entire program in MIPS - called externally to
    *               translate the algol parsed code into MIPS assembly code.
*/
void EMIT(ASTnode *p, FILE *fp);

/*  * @precondition: p points to an A_WRITE node, fp points to the outfile
    * @postcondition:
    * @param: ASTnode *p & FILE *fp
    * @description: This function will print all static strings from program to MIPS file under .data
    *               section.
*/
void emitStrings(ASTnode *p, FILE *fp);

/*  * @precondition: p points to an A_VARDEC node, fp points to the outfile
    * @postcondition: generates MIPS code
    * @param: ASTnode *p & FILE *fp
    * @description: This function will print all global variables
*/
void emitGlobals(ASTnode *p, FILE *fp);


#endif
