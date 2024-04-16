%{
/*          Trang Tran
            September 25, 2023

            **Description & Purpose::
                This LEX program will parse through user inputed character stream and assign
                tokens to what was inputted. Lowercase letters are assigned the VARIABLE token.
                Numerical characters are assigned the INTEGER token. Operators and carriage
                returns are assigned themselves as tokens. It will then pass the tokens to
                the lab4.y file to compute.

              - Input:  Character stream from users are inputted. We assume the user will be
                        inputting equations that a simple calculator can compute.
              - Output: Either tokens or token-value pairs are outputted.

            **Changes made to document by TRANG TRAN on AUGUST 23, 2023**
              --  Added inline comments and documentation prior to functions.
              --  Added '()' to line 58 in order to include parenthesis as a token that LEX will
                  pass onto the y file.
              --  Standardized and spellchecked the debug print statements

            **Changes made to document by TRANG TRAN on SEPTEMBER 6, 2023**
              --  Added 'int' to regex rules. This will return a T_INT token that communicates
                  to yacc that there is an integer variable being declared.
              --  Added ';' to regex rules. This will communicate to yacc when a variable
                  declaration is over.
              --  Added global variable linecount to keep track of lines that have been entered.
                  This variable will be used in yacc file to output error messages that refer to a
                  specific line of input.

            ** Changes made to document by TRANG TRAN on SEPTEMBER 13, 2023**
              --  Expanded the alphabet for LEX and YACC by adding new tokens.
              --  Added 'begin', 'end', 'return', 'boolean', 'void', 'if', 'then', 'else', 'endif',
                  'while', 'do', 'read', 'write', 'or', 'and', 'true', 'false', 'not', 'continue',
                  and 'break' tokens that are recognized when literal strings are entered.
              --  Added operators as separate tokens that will return to yacc.
              --  Added T_STRING token that is paired with a 'string' value through union% function
                  in yacc. This allows the value of the input string to be handed off to yacc.
              --  Deleted the return when a new line character is found
              --  Added parenthesis, commas, and square brackets to catch-all operator token in line
                  198.
              --  Added '"//".*' to ignore all lines with comments in them.

            ** Changes made to document by TRANG TRAN on September 23, 2023**
              -- Added an import statement for ast.h file.
*/

//Global variable mydebug will print out statements that show what type of tokens have been
//found by LEX from the input.
int mydebug=0;
//Global variable linecount will keep track of new line characters
int linecount = 1;
//Preprocessing, includes the h file that was compiled from the lab4.y file.
#include "ast.h"
#include "y.tab.h"
#include "symtable.h"
%}

%%
 /* Start of LEX rules*/

    /* 'begin' and 'end' are used to determine the start of a function call, an
       iterative statement or an if/else statement. */
begin                   {if (mydebug) fprintf(stderr,"T_BEGIN token Found\n");
                            return(T_BEGIN);
                        }
end                     {if (mydebug) fprintf(stderr,"T_END token Found\n");
                            return(T_END);
                        }
    /*  'return' is used at the end of a function call to return values */
return                  {if (mydebug) fprintf(stderr,"T_RETURN token Found\n");
                            return(T_RETURN);
                        }
    /*  'int', 'boolean', and 'void' are all value types used in declaring a function
        or a variable. They are not paired with actual values. */
int                     { if (mydebug) fprintf(stderr,"T_INT token Found\n");
                            return(T_INT);}           //If regex found 'int', the token T_INT is returned to yacc to process a variable declaration
boolean                 {if (mydebug) fprintf(stderr,"T_BOOLEAN token Found\n");
                            return(T_BOOLEAN);
                        }
void                    {if (mydebug) fprintf(stderr,"T_VOID token Found\n");
                            return(T_VOID);
                        }
    /*  'if', 'then', 'else', and 'endif' are all used to determine when an if-statement is
        begins, when to start the expression after a relative operation is complete, whether or
        not there is an else statement that follows, and when the if/if-else statement ends. */
if                      {if (mydebug) fprintf(stderr,"T_IF token Found\n");
                            return(T_IF);
                        }
then                    {if (mydebug) fprintf(stderr,"T_THEN token Found\n");
                            return(T_THEN);
                        }
else                    {if (mydebug) fprintf(stderr,"T_ELSE token Found\n");
                            return(T_ELSE);
                        }
endif                   {if (mydebug) fprintf(stderr,"T_ENDIF token Found\n");
                            return(T_ENDIF);
                        }
    /*  'while' and 'do' are tokens that signal when an iterative statement will begin and
        what the program should do in the duration of the loop. */
while                   {if (mydebug) fprintf(stderr,"T_WHILE token Found\n");
                            return(T_WHILE);
                        }
do                      {if (mydebug) fprintf(stderr,"T_DO token Found\n");
                            return(T_DO);
                        }
    /*  'read' and 'write' tokens signal input/output starts. */
read                    {if (mydebug) fprintf(stderr,"T_READ token Found\n");
                            return(T_READ);
                        }
write                   {if (mydebug) fprintf(stderr,"T_WRITE token Found\n");
                            return(T_WRITE);
                        }
    /*  The relative operators and mathematical operators are tokenized to represent themselves. */
"<"                     {if (mydebug) fprintf(stderr,"T_LE token Found\n");
                            return(T_LE);
                        }
">"                     {if (mydebug) fprintf(stderr,"T_GR token Found\n");
                            return(T_GR);
                        }
"<="                    {if (mydebug) fprintf(stderr,"T_LEEQU token Found\n");
                            return(T_LEEQU);
                        }
">="                    {if (mydebug) fprintf(stderr,"T_GREQU token Found\n");
                            return(T_GREQU);
                        }
"=="                    {if (mydebug) fprintf(stderr,"T_EQUAL token Found\n");
                            return(T_EQUAL);
                        }
"!="                    {if (mydebug) fprintf(stderr,"T_NEQU token Found\n");
                            return(T_NEQU);
                        }
"+"                     {if (mydebug) fprintf(stderr,"T_PLUS token Found\n");
                            return(T_PLUS);
                        }
"-"                     {if (mydebug) fprintf(stderr,"T_MINUS token Found\n");
                            return(T_MINUS);
                        }
"*"                     {if (mydebug) fprintf(stderr,"T_MUL token Found\n");
                            return(T_MUL);
                        }
"/"                     {if (mydebug) fprintf(stderr,"T_DIV token Found\n");
                            return(T_DIV);
                        }
"="                     {if (mydebug) fprintf(stderr,"T_ASSIGN token Found\n");
                            return(T_ASSIGN);
                        }
    /*  'or', 'and' & 'not' tokens represent logical operators.  */
or                      {if (mydebug) fprintf(stderr,"T_OR token Found\n");
                            return(T_OR);
                        }
and                     {if (mydebug) fprintf(stderr,"T_AND token Found\n");
                            return(T_AND);
                        }
not                     {if (mydebug) fprintf(stderr,"T_NOT token Found\n");
                            return(T_NOT);
                        }
    /*  'true' and 'false' are tokens of themselves. */
true                    {if (mydebug) fprintf(stderr,"T_TRUE token Found\n");
                            return(T_TRUE);
                        }
false                   {if (mydebug) fprintf(stderr,"T_FALSE token Found\n");
                            return(T_FALSE);
                        }

    /*  'continue' and 'break' represent themselves. */
continue                {if (mydebug) fprintf(stderr,"T_CONT token Found\n");
                            return(T_CONT);
                        }
break                   {if (mydebug) fprintf(stderr,"T_BREAK token Found\n");
                            return(T_BREAK);
                        }
    /*  T_STRING represents a string literal. String literals can be any character or any length, but must
        appear between quotation marks. A pointer to the string is returned.*/
\".*\"                  {if (mydebug) fprintf(stderr,"T_STRING Found: %s\n", yytext);
                            yylval.string=strdup(yytext);                               //Assigns the token value T_STRING if a string of letters is found
                            return(T_STRING);
                        }
    /*  T_ID represents a variable name. A variable must start with a letter, but can be followed by letters,
        numbers or an underscore. A pointer to the string is returned*/
[A-Za-z][A-Za-z0-9_]*	{
                        if (mydebug) fprintf(stderr,"T_ID Found: %s\n",yytext);
                            yylval.string=strdup(yytext);                                  //Assigns the token value T_ID if a string of letters is found
                            return(T_ID);
                        }
    /*  T_NUM is a numerical value with integer type. A pointer to the value is returned */
[0-9][0-9]*	            {if (mydebug) fprintf(stderr,"T_NUM Found %s\n",yytext);
                            yylval.value=atoi((const char *)yytext); //Assigns the token value T_NUM if a numerical character is found
                            return(T_NUM);
                        }
    /*  The following is a 'catch-all' for operators used in the alphabet. The tokens returned are the operator
        themselves. */
[/%(),[\]]       	    { if (mydebug) fprintf(stderr,"Operator Found %c\n",*yytext);
                            return (*yytext);
                        }
    /*  The semi-colon is a token in iself, and will be returned as such. */
;                       {if (mydebug) fprintf(stderr, "Semicolon Found. Variable declaration complete \n");
                            return (*yytext);
                        }
    /*  When a newline character is found, LEX will increment the linecount variable but not return anything
        to YACC. */
\n 		                { if (mydebug) fprintf(stderr,"Carriage return found %c\n",*yytext);
                            linecount++;
                        }
    /*  Whitespace is disregarded in our alphabet, and LEX will not return anything to YACC. */
[ \t]		            {if (mydebug) fprintf(stderr,"Whitespace Found\n");
                        }
    /*  The comment token will allow LEX to ignore the rest of the line and
        thus nothing is returned to YACC.  */
"//".*                  {if (mydebug) fprintf(stderr,"A commented line has occurred\n");
                        }
%%
/* end of rule */

/* * @precondition: None
    * @postcondition: None
    * @param: None
	  * @return: Returns '1' if True
    * @description:
*/
int yywrap(void)
{//start of yywrap
  return 1;
  //end of yywrap
}
