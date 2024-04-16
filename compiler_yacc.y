%{
/* --------------------------------------------------------------------------------------------------------------------
            Trang Tran
            November 8, 2023

            **Description & Purpose::
              This program will identify all tokens that have been passed from LEX to YACC and determine if
			  the input is valid in terms of the production rules below. It uses the production rules according to the
			  BNF and implements both a symbol table and abstract symbol tree in order to build and print the grammar.
			  The ast.h file will connect this file with the abstract symbol tree print function. The symtable.h file
			  will connect us to the symbol table. In the syntax directed statements of each production rule, nodes of the
			  abstract symbol tree are created and linked to one another. When there is a declaration (of either variable
			  or function), the syntax directed statements in each production rule will check to make sure this is a
			  valid declaration.

              - Input:  The tokenized stream that LEX parsed from the user inputted character stream.
              - Output: The abstract symbol tree and symbol table for the inputted program is printed to the terminal.
			  			If the inputted program has invalid declarations or invalid use of rules, the user will be notified
						of the line in which the error occurred and what type of error it is.

    *********************Changes made to document by TRANG TRAN on AUGUST 23, 2023*********************
              --  Added inline comments and documentation prior to functions.
			  --  Added multiplication expression evaluation. (line 155)
			  --  Removed the the first 'expr' in front of the '-' for the unary minus expression. (line 167)
			  --  Added the parenthesis expression evaluation. (line 143)
			  --  Added the yylex() function to insure that there is no warning from compiler (line 73)
			  --  Proofread and cleaned up print statements
			  --  Corrected the ouput for variables found in line 172 to include the correct value.

	*********************Changes made to document by TRANG TRAN on SEPTEMBER 6, 2023*********************
			  --  Added global variable MAX to set the size of the symbol table and regs[] size is set to MAX
			  --  Linked linecount variable from lex file.
			  --  Added global variable 'CURRENT' to hold the current index in symbol table
			  --  Added union function to pair tokens with the appropriate data type
			  --  Defined which tokens need data types specification
			  --  Changed start of rules from 'list' to 'P'
			  --  Added rules P, DCLS, DCL so that variables can be declared and saved in the symbol table.
			  --  Stat rule is updated to search for whether a VARIABLE has been declared or not. If
			  	  it has been declared, the address will be used to get the value of the VARIABLE. If the
				  VARIABLE has not been declared, an error message will printed.
			  --  expr rule is updated to search for whether a VARIABLE has been declared before
			      its value can be used to compute an entire expression. If it has not been declared,
				  the value is set to '0' and an error message will be printed.

	*********************Changes made to document by TRANG TRAN on SEPTEMBER 16, 2023*********************
			  --  All tokens have been added to the document.
			  --  All production rules have been added to the document.
			  --  Continue, Break and PrototypeDescription production rules have been added to document to reflect
			  	  what is expected of graduate students.
			  --  None of the rules have functionality in terms of a calculator anymore or any real functionatlity other
			  	  than to set up an alphabet that can be expanded upon.
			  --  mydebug and linecount are external variables that LEX controls but is used in YACC file to output
			  	  the line number an error occurs and to turn the debug print statements on.

	*********************Changes made to document by TRANG TRAN on September 22, 2023************************
			  --  'ast.h' is included into the document
			  --  datatypes ASTnode*, enum DataTypes, and enum OPERATORS are added into the %union function.
			  	  This will connect the yacc file and the 'ast.c' file.
			  --  %type <node>, %type <datatype>, and %type <operator> are declare production rules as specific datatypes
			      in the abstract symbol tree.
			  --  All production rules have been assigned to nodes of the same name and all data for each production
			      rule is stored in either pointers s1, s2, operator, name, or value as class properties of the object
				  ASTnode.
	*********************Changes made to document by TRANG TRAN on October 20, 2023************************
			  --  PrototypeDeclaration production rule was deleted. Instead, FunctionBody production rule was added. This
			  	  allows us to have the same parse for the declaration of a function - but then decide whether or not
				  this first pattern belongs to a full function definition or just a prototype declaration.
			  --  Global variables level, offset, goffset and maxoffset are declared to be used in the symbol table.
			  --  Included the 'symtable.h' file in order to link the code that builds a symbol table.
			  --  Added syntax-directed instructions to rules that declare, define and use variables and functions - see
			      each rule for specific changes made to rule.
	*********************Changes made to document by TRANG TRAN on  November 29, 2023************************
			  --  Added 'param' flag in symbol table. It is only used in the case that an array was passed as a parameter.
			  	  The flag is detected in MIPS and signals when to load an array address' value into $a0. This flag is only
				  for parameters.
			  --  Added var_check() to 'write' rule so that there is no illegal use of an A_VAR.
			  --  Left factored Varlist rule
			  --  Added commands in main to print MIPS code into an .asm file


*/

	/* begin specs */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "ast.h"
#include "symtable.h"
#include "emit.h"

extern int linecount;
extern int mydebug;

int level = 0; 	//This keeps track of how many compound statements deep we are in
int offset = 0; 	//This is how many words have we seen at a global or inside a function
int goffset; 	// Holder for global offset when we enter and exit function definition
int maxoffset; 		//total number of words a function needs

/*  * @precondition: There is input to be evaluated.
    * @postcondition: None
    * @param: None
	* @return: Returns the type of token that was found in LEX
    * @description: */
int yylex();

/* @description: This function will be called if there is an error */
void yyerror (s)  /* Called by yyparse on error */ //start yyerror
     char *s;
{
  printf ("Error found in line %d: %s \n", linecount, s);
} //end yyerror
%} //end of C

/*  defines the start symbol, what values come back from LEX and how the operators are associated  */

%start Program

/* This function allows tokens to be paired with typevalues.
   -- September 22: Added types ASTnode*, enum DataTypes and enum OPERATORS */
%union{
	int value;
	char* string;
	ASTnode* node;
	enum DataTypes datatype;
	enum OPERATORS operator;
} /*end union*/

/* T_NUM, T_ID and T_STRING are paired with specific value types. */
%token <value> T_NUM
%token <string> T_ID
%token <string> T_STRING

/* T_INT, T_BOOLEAN and T_VOID are variable types */
%token	T_INT T_BOOLEAN T_VOID

/* The following tokens are represented in lex as literal strings and will determine which production
rule will be triggered. */
%token	T_BEGIN T_END T_RETURN
%token	T_IF T_THEN T_ELSE T_ENDIF
%token	T_WHILE T_DO
%token	T_READ T_WRITE
%token	 T_TRUE T_FALSE T_NOT T_OR T_AND
%token	T_CONT T_BREAK

/* The following tokens represent operation symbols */
%token	T_LE T_GR T_LEEQU T_GREQU T_NEQU T_EQUAL
%token	T_PLUS T_MINUS T_MUL T_DIV T_ASSIGN

/* The following are declarations of nodes on the abstract symbol tree in ast.c */
%type <node> Declaration DeclarationList VarList VarDeclaration FunDeclaration FunctionBody
%type <node> ParamList Param Params LocalDeclarations StatementList Statement
%type <node> ExpressionStmt CompoundStmt SelectionStmt IterationStmt AssignmentStmt ReturnStmt ReadStmt WriteStmt ContinueStmt BreakStmt
%type <node> Expression Var SimpleExpression AdditiveExpression Term Factor Call ArgList Args

/* The following will connect the datatypes to nodes in the abstract symbol tree in ast.c*/
%type <datatype> TypeSpecifier

/* The following will connect the operators to nodes in the abstract symbol tree in ast.c.*/
%type <operator> Addop Relop Multop

%%	/* end specs, begin rules */

/*	This starts our YACC parsing.
    Added September 22: program, a global node variable from ast.h is set to Program production rule.
					 This is the root node of the abstract symbol tree.
*/
Program 			: 	DeclarationList
						{	if(mydebug) fprintf(stderr, "Program Production Rule\n");
							if (Has_Proto() == 1){
								yyerror("There is an undefined function.");
								exit(1);
							}
							program = $1;
						}
					;

/* @description: 	DeclarationList production rule is a recursive rule that calls the Declaration rule and itself.
					Added September 22: DeclarationList production rule is set to node A_DEC_LIST and its $1 and $2 values
								are set to either s1 or s2 pointers of the node.
*/
DeclarationList		: 	Declaration
						{	if(mydebug) fprintf(stderr, "DeclarationList Production Rule\n");
							$$ = ASTCreateNode(A_DEC_LIST);
							$$ -> s1 = $1;
						}
					|	Declaration DeclarationList
						{	if(mydebug) fprintf(stderr, "Declaration DeclarationList Production Rule \n");
							$$ = ASTCreateNode(A_DEC_LIST);
							$$ -> s1 = $1;
							$$ -> s2 = $2;
						}
					;

/*	@description: 	Declaration production rule calls one of three types of declarations - VarDeclaration or FunDeclaration.
*/
Declaration			:	VarDeclaration
						{	if(mydebug) fprintf(stderr, " Declaration VarDeclaration Production Rule\n");
							$$ = $1;
						}
					| 	FunDeclaration
						{	if(mydebug) fprintf(stderr, " Declaration FunDeclaration Production Rule\n");
							$$ = $1;
						}
					;

/*	@description: 	VarDeclaration defines how a variable must be declared.
					Added September 22: VarDeclaration is set to the $2. The ASTnode *p is a pointer to the varList in $2.
								p's datatype is set to TypeSpecifier and p is set its own s1 pointer.
*/
VarDeclaration		:	TypeSpecifier VarList ';'
						{	if(mydebug) fprintf(stderr, " VarDeclaration Production Rule\n");
							$$ = $2;
							$$ -> datatype = $1;

							/*set datatype*/
							ASTnode *p;
							p = $2;
							while (p != NULL){
								p->symbol->Declared_Type = $1;
								p = p->s1;
							} //end while
						}
					;

/*	@description: 	VarList can be described with one of the four identifiers in its rule list. It can also be recursive
					to include multiple variables.
					Added September 22: Each production rule in VarList is set to its own A_VARDEC node depending on which
								rule is triggered by the tokens found in LEX. Every node has a name that is set and
								if it is an array declaration, the array's length is set to the node's value data.
					Added October 20: For each production rule, before a node is created for the AST, we search the symbol
								table to see if it already exists. If it does, an error is printed and the program is exited.
								If not, the node is created, the offset (the size of the variable) is set and the variable
								is added into the symbol table. The offset of an array is set to be the current offset + the
								size of the array.
*/
VarList				:	T_ID
						{	if(mydebug) fprintf(stderr, " VarList T_ID Production Rule\n");
							/*fprintf(stderr, "VarList T_ID Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", $1);*/

							if (Search($1, level, 0) == NULL){
								//symbol not there, put it in the symbol table
								$$ = ASTCreateNode(A_VARDEC);
								$$ -> name = $1;
								$$ -> value = 0;
								$$ -> symbol = Insert($1, A_UNKNOWN, SYM_SCALAR, level, 1, offset);
								offset = offset + 1;
							}
							else {
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}
						}

					| 	T_ID '[' T_NUM ']'
						{	if(mydebug) fprintf(stderr, " VarList T_ID [T_NUM] Production Rule\n");
							/*fprintf(stderr, "VarList T_ID [T_NUM] Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", $1);*/
							if (Search($1, level, 0) == NULL){
								//symbol not there, put it in the symbol table
								$$ = ASTCreateNode(A_VARDEC);
								$$ -> name = $1;
								$$ -> value = $3;
								$$ -> symbol = Insert($1, A_UNKNOWN, SYM_ARRAY, level, $3, offset);
								offset = offset + $3;
							}
							else {
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}
						}

					| 	VarList ',' T_ID
						{	if(mydebug) {
								fprintf(stderr, " VarList T_ID , VarList Production Rule\n");
								fprintf(stderr, "VarList T_ID , VarList Production Rule\n");
								fprintf(stderr, "	T_ID: %s\n", $1);
							}
							if (Search($3, level, 0) == NULL){
								//symbol not there, put it in the symbol table
								$$ = ASTCreateNode(A_VARDEC);
								$$ -> name = $3;
								$$ -> s1 = $1;
								$$ -> value = 0;
								$$ -> symbol = Insert($3, A_UNKNOWN, SYM_SCALAR, level, 1, offset);
								offset = offset + 1;
							}
							else {
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}
						}

					| 	VarList ',' T_ID '[' T_NUM ']'
						{	if(mydebug) {
								fprintf(stderr, " VarList T_ID [T_NUM], Varlist Production Rule\n");
								fprintf(stderr, "VarList T_ID [T_NUM], Varlist Production Rule\n");
								fprintf(stderr, "	T_ID: %s\n", $1);
							}

							if (Search($3, level, 0) == NULL){
								//symbol not there, put it in the symbol table
								$$ = ASTCreateNode(A_VARDEC);
								$$ -> name = $3;
								$$ -> value = $5;
								$$ -> s1 = $1;
								$$ -> symbol = Insert($3, A_UNKNOWN, SYM_ARRAY, level, $5, offset);
								offset = offset + $5;
							}
							else {
								yyerror($1);
								yyerror("Already defined");
								exit(1);
							}
						}
					;

/*	@description: 	TypeSpecifier production rule is decided by one of the following tokens.
					Added September 22: Every token type is set to a corresponding datatype that will be set in an ASTnode*.
*/
TypeSpecifier		:	T_INT
						{	if(mydebug) fprintf(stderr, " TypeSpecifier T_INT Production Rule\n");
							$$ = A_INTTYPE;
						}
					|	T_VOID
						{	if(mydebug) fprintf(stderr, " TypeSpecifier T_VOID Production Rule\n");
							$$ = A_VOIDTYPE;
						}
					|	T_BOOLEAN
						{	if(mydebug) fprintf(stderr, " TypeSpecifier T_BOOLEAN Production Rule\n");
							$$ = A_BOOLEANTYPE;
						}
					;

/*	@description: 	FunDeclaration production rule defines how a function must be declared.
					Added September 22: FunDeclaration is set to ASTnode A_FUNDEC. T_ID is set to the name of the function,
							     TypeSpecifier is set to datatype, and Params and CompoundStmt are set to pointer s1 & s2.
					Added October 20: FunDeclaration's rule has been split into sections. The first section will search
								for whether the name of the function has already been declared elsewhere or not - if it has
								an error is thrown and the program exited. If it has not, the function is entered into the
								symbol table. Functions have a size of 2 in our symbol table, and thus the offset is set to 2.
								The function's parameters are set to the fparms class variable for the symbol in the table.
								fparms is an ASTnode pointer and will eventually link the tree to symbol table once the node
								is created. At the FunctionBody, we are inputting the values associated to either a function
								or prototype declaration node.
*/
FunDeclaration		:	TypeSpecifier T_ID '('
					{	//check to see if function name is known
						// insert into symbol table unless it already exists
						//manage offset values

						struct SymbTab *p;
						p = Search($2, level, 0);

						if (p == NULL){
							Insert($2, $1, SYM_FUNCTION, level, 0, 0);
						}

						else{
							if (p -> SubType == SYM_FUNCTION_PRE){
								p -> SubType = SYM_FUNCTION_PROTO;
							}

							else {
							printf("FUNCTION SUBTYPE:: %d\n", p -> SubType);
							yyerror($2);
							yyerror("Can't create function. Name in use \n");
							exit(1);}

						}
						goffset = offset;
						offset = 2; 								//2 for SP and RA
						maxoffset = offset;							//maxoffset = largest amount of data needed
					}

					Params ')'
					{
						struct SymbTab *p;
						p = Search($2, level, 0);

						if (p -> SubType == SYM_FUNCTION_PROTO){
							//check params
							if (proto_func_check(p->fparms, $5) == 0){
								yyerror("Function cannot be declared. Parameters do not match.");
								exit(1);
							}
							//p -> SubType = SYM_FUNCTION;
						}

						else {
							//update symbtable with parameter
							// allows us to have recursive functions
							Search($2, level, 0)->fparms = $5;
						}
					}

					FunctionBody
					{	if(mydebug) {
							fprintf(stderr, " FunDeclaration Production Rule\n");
							fprintf(stderr, "FunDeclaration Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", $2);}

						struct SymbTab *p;
						p = Search($2, level, 0);

						$$ = $8;
						$$ -> name = $2;
						$$ -> datatype = $1;
						$$ -> s1 = $5;
						$$ -> symbol = Search($2, level, 0);
						$$ -> symbol -> offset = maxoffset;
						offset = goffset;
						maxoffset = offset;

						if(p->SubType == SYM_FUNCTION_PROTO){
							if ($$->s2 == NULL){
								yyerror("Failed to add function body.");
								exit(1);
							}
							p->SubType = SYM_FUNCTION;
						}

						if ($$ -> s2 == NULL){
							$$ -> symbol -> SubType = SYM_FUNCTION_PRE;
							$$ -> symbol -> offset = 0;
							//deleting the symbols from the symbol table will allow us to create the function and
							Delete(level+1);
						}
					}
					;

/*	@description: FunctionBody represents either a full function definition or a prototype of a function being declared.
					Prior to October 20, prototype declarations were their own set of production rules. As of October 20,
					the FunctionBody rule will decipher between prototype and a fully defined function. This allows us to
					manage the symbol table in a more efficient way.
*/
FunctionBody:		';'
					{	if(mydebug) {
							fprintf(stderr, " PrototypeDeclaration Production Rule\n");
							fprintf(stderr, "PrototypeDeclaration Production Rule\n");
							}
							$$ = ASTCreateNode(A_PROTODEC);
					}

					| CompoundStmt
					{	if(mydebug) {
							fprintf(stderr, " PrototypeDeclaration Production Rule\n");
							fprintf(stderr, "PrototypeDeclaration Production Rule\n");
							}
							$$ = ASTCreateNode(A_FUNDEC);
							$$ -> s2 = $1;
					}
					;

/*	@description:	Params represent parameters in a fuction declaration or call. It can either
					be a list of parameters or void.
					Added September 22: This productioni rule creates an A_PARAMS node and holds either a
					'void' datatype or a parameter list set to the s1 pointer.
					Changed October 20: The $$ in ParamList refers directly to the parameter list, instead of creating
										an A_PARAMS node that holds the parameter list.
*/
Params				:	T_VOID
					{	if(mydebug) fprintf(stderr, " Params T_VOID Production Rule\n");
						$$ = ASTCreateNode(A_PARAMS);
						$$ -> datatype = A_VOIDTYPE;
					}
					|	ParamList
					{	if(mydebug) fprintf(stderr, " Params ParamList Production Rule\n");
						$$ = $1;
					}
					;

/*	@description:	ParamList is a recursive production rule that can contain one Param or many.
					Added September 22: This production rule creates an A_PARAMLIST node and holds one
					A_PARAM node in its s1 pointer or A_PARAM node in s1 pointer and A_PARAMLIST node in its s2
					pointer.
					Changed October 20: The ParamList leftside nonterminal now refers directly to the PARAM nonterminal.
*/
ParamList			:	Param
					{	if(mydebug) fprintf(stderr, " ParamList Param Production Rule\n");
						$$ = $1;
					}

					|	Param ',' ParamList
					{	if(mydebug) fprintf(stderr, " ParamList Param , ParamList Production Rule\n");
						$$ = $1;
						$$ -> s2 = $3;
					}
					;

/*	@description:	A param is defined as either a variable with its type or a variable array.
					Added September 22: This production rule creates A_PARAM node and sets the name and datatype
					of the parameter to pointers name and datatype.
					Added October 20: A parameter node is created. The name of the parameter is searched for in the symbol
									table - if it already exists at the current level, an error is thrown and the program
									is exited. The value of the node type is set to -1 if it is an array and 0 if it is not.
*/
Param				: TypeSpecifier T_ID
					{	if(mydebug) {
							fprintf(stderr, " Param TypeSpecifier T_ID Production Rule\n");
							fprintf(stderr, "Param TypeSpecifier T_ID Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", $2);
						}
						$$ = ASTCreateNode(A_PARAM);
						$$ -> name = $2;
						$$ -> datatype = $1;
						$$ -> value = 0;

						if (Search($2, 1, 0) == NULL){
							$$ -> symbol = Insert($2, $1, SYM_SCALAR, 1, 1, offset);
							offset = offset + 1;
							}
						else {
								yyerror($2);
								yyerror("Already defined parameter");
								exit(1);
							}
					}

					| TypeSpecifier T_ID '[' ']'
					{	if(mydebug) {
							fprintf(stderr, " Param TypeSpecifier T_ID [] Production Rule\n");
							fprintf(stderr, "Param TypeSpecifier T_ID [] Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", $2);
						}

						$$ = ASTCreateNode(A_PARAM);
						$$ -> name = $2;
						$$ -> datatype = $1;
						$$ -> value = -1;

						if (Search($2, 1, 0) == NULL){
							$$ -> symbol = Insert($2, $1, SYM_ARRAY, 1, 1, offset);
							$$ -> symbol -> param = 1;
							offset = offset + 1;
							}
						else {
								yyerror($2);
								yyerror("Already defined parameter");
								exit(1);
							}
					}
					;

/*	@description:	A CompoundStmt represents the start of a different scope in the alphabet. Basically it is the curly braces
					that start and end function calls, if/else statements, and while-loops that have more than one
					statement in them.
					Added September 25: This production rule creates an A_COMPOUND node and sets the LocalDeclarations to
					the s1 pointer and StatementList to the s2 pointer.
					Added October 20: After the T_BEGIN token, the level is incremented by one. When we leave the compound statement
									at the T_END token, we reduce the level by one and delete all the variables in the
									level and reduce the offset by the total offset in the level we are deleting.
*/
CompoundStmt		:	T_BEGIN
					{
						level = level + 1;
					}

					LocalDeclarations StatementList T_END
					{	if(mydebug) fprintf(stderr, " CompoundStmt Production Rule\n");
						$$ = ASTCreateNode(A_COMPOUND);
						$$->s1 = $3;
						$$->s2 = $4;
						if (mydebug) Display();
						if (offset > maxoffset)
							maxoffset = offset;
						offset = offset - Delete(level);
						level = level - 1;
					}
					;

/*	@description:	LocalDeclarations is a right-recursive call that specifies the scope in which a variable declaration occurs.
					It is called within a CompoundStmt.
					Added September 25: This production rule is set to NULL for an empty list or creates an A_LOCALDEC node which
					holds a VarDeclaration in the s1 pointer and more LocalDeclarations in its s2 pointer.
*/
LocalDeclarations	:	/*empty*/
					{	if(mydebug) fprintf(stderr, " LocalDeclarations Production Rule\n");
						$$ = NULL;
					}

					|	VarDeclaration LocalDeclarations
					{	if(mydebug) fprintf(stderr, " LocalDeclarations VarDeclaration LocalDeclarations Production Rule\n");
						$$ = ASTCreateNode(A_LOCALDEC);
						$$->s1 = $1;
						$$ -> s2 = $2;}
					;

/*	@description:	StatementList is a right-recursive function that is either empty, has a statement or has multiple statements.
					Added September 25: This production rule is set to NULL or creates an A_STMTLIST node which stores an instance of
					Statement in the s1 pointer or another instance of itself in the s2 pointer.
*/
StatementList		:	/*empty*/
					{	if(mydebug) fprintf(stderr, " StatementList Production Rule\n");
						$$ = NULL;
					}

					|	Statement StatementList
					{	if(mydebug) fprintf(stderr, " StatementList Statement StatementList Production Rule\n");
						$$ = ASTCreateNode(A_STMTLIST);
						$$ -> s1 = $1;
						$$ -> s2 = $2;
					}
					;

/*	@description:	Statement production rule lists the different statements that can be called in our program. */
Statement			:	ExpressionStmt
					{	if(mydebug) fprintf(stderr, " Statement ExpressionStmt Production Rule\n");
						$$ = $1;
					}

					|	CompoundStmt
					{	if(mydebug) fprintf(stderr, " Statement CompoundStmt Production Rule\n");
						$$ = $1;
					}

					|	SelectionStmt
					{	if(mydebug) fprintf(stderr, " Statement SelectionStmt Production Rule\n");
						$$ = $1;
					}

					|	IterationStmt
					{	if(mydebug) fprintf(stderr, " Statement IterationStmt Production Rule\n");
						$$ = $1;
					}

					|	AssignmentStmt
					{	if(mydebug) fprintf(stderr, " Statement AssignmentStmt Production Rule\n");
						$$ = $1;
					}

					|	ReturnStmt
					{	if(mydebug) fprintf(stderr, " Statement ReturnStmt Production Rule\n");
						$$ = $1;
					}

					|	ReadStmt
					{	if(mydebug) fprintf(stderr, " Statement ReadStmt Production Rule\n");
						$$ = $1;
					}

					|	WriteStmt
					{	if(mydebug) fprintf(stderr, " Statement WriteStmt Production Rule\n");
						$$ = $1;
					}

					|	ContinueStmt
					{	if(mydebug) fprintf(stderr, " Statement ContinueStmt Production Rule\n");
						$$ = $1;
					}

					|	BreakStmt
					{	if(mydebug) fprintf(stderr, " Statement BreakStmt Production Rule\n");
						$$ = $1;
					}
					;

/*	@description:	ExpressionStmt is defined as an expression followed by a semicolon, or just a semicolon.
					Added September 25: This production rule creates an A_EXPRSTMT node that holds either
					an A_EXPR node in its s1 pointer.
*/
ExpressionStmt		:	Expression ';'
					{	if(mydebug) fprintf(stderr, " ExpressionStmt Expression Production Rule\n");
						$$ = ASTCreateNode(A_EXPRSTMT);
						$$ -> s1 = $1;
						if (var_check($1) == 0){
							yyerror("Variable use invalid");
							exit(1);
						}
					}

					| 	';'
					{	if(mydebug) fprintf(stderr, " \n");
						$$ = ASTCreateNode(A_EXPRSTMT);
					}
					;

/*	@description:	SelectionStmt is triggered by an T_IF token and ended with a T_ENDIF statement.
					The T_ELSE token is an option.
					Added September 25: This production rule creates an A_SELECTSTMT node and an A_IFBODY node. The
					A_SELECTSTMT node holds the if-condition in the s1 pointer for both the if and if-else branch.
					A_IFBODY holds the statements after the T_THEN token in the s1 pointer and if needed, the statement
					after then T_ELSE token in the s2 pointer. A_SELECTSTMT's s2 pointer is then pointed to the A_IFBODY.
*/
SelectionStmt		:	T_IF Expression T_THEN Statement T_ENDIF
					{	if(mydebug) fprintf(stderr, " SelectionStmt T_IF Production Rule\n");
						$$ = ASTCreateNode(A_SELECTSTMT);
						ASTnode* ifBody = ASTCreateNode(A_IFBODY);
						ifBody-> s1 = $4;
						$$ -> s2 = ifBody;
						$$ -> s1 = $2;
					}

					|	T_IF Expression T_THEN Statement T_ELSE Statement T_ENDIF
					{	if(mydebug) fprintf(stderr, " SelectionStmt T_IF T_ELSE Production Rule\n");
						$$ = ASTCreateNode(A_SELECTSTMT);
						ASTnode* ifBody = ASTCreateNode(A_IFBODY);
						ifBody -> s1 = $4;
						ifBody -> s2 = $6;
						$$ ->s1 = $2;
						$$ -> s2 = ifBody;
						}
					;

/*	@description:	IterationStmt will invoke a while loop.
					Added September 27: This creates an A_ITERSTMT node. The while-condition is set to the s1 pointer
					and the do-statement is set to the s2 pointer.
*/
IterationStmt		:	T_WHILE Expression T_DO Statement
					{	if(mydebug) fprintf(stderr, " IterationStmt Production Rule\n");
						$$ = ASTCreateNode(A_ITERSTMT);
						$$ -> s1 = $2;
						$$ -> s2 = $4;
					}
					;

/*	@description:	ReturnStmt will either return nothing or an expression.
					Added September 25: The return statement is set to NULL or an A_RETURN node is created. It's s1
					pointer holds the expression to be returned.
*/
ReturnStmt			:	T_RETURN ';'
					{	if(mydebug) fprintf(stderr, " ReturnStmt T_RETURN Production Rule\n");
						$$ = ASTCreateNode(A_RETURN);
					}
					|	T_RETURN Expression ';'
					{	if(mydebug) fprintf(stderr, " ReturnStmt T_RETURN Expression Production Rule\n");
						$$ = ASTCreateNode(A_RETURN);
						$$ -> s1 = $2;
					}
					;

/*	@description:	ReadStmt will read the variable inputted.
					Added September 25: This production rule creates an A_READ node and will store the
					statement following the T_READ token in the s1 pointer.
*/
ReadStmt			:	T_READ Var ';'
					{	if(mydebug) fprintf(stderr, " ReadStmt Production Rule\n");
						$$ = ASTCreateNode(A_READ);
						$$ -> s1 = $2;
					}
					;

/*	@description:	WriteStmt will either write an expression or T_STRING to the terminal.
					Added September 27: This production rule creats an A_WRITE node and will store either the expression
					to be written in s1 node or the input string to be written in the name property.
*/
WriteStmt			:	T_WRITE Expression ';'
					{	if(mydebug) fprintf(stderr, " WriteStmt T_WRITE Expression Production Rule\n");

						if (var_check($2) == 0){
							yyerror("Variable use invalid");
							exit(1);
						}

						$$ = ASTCreateNode(A_WRITE);
						$$->s1 = $2;
					}

					|	T_WRITE T_STRING ';'
					{	if(mydebug) {
						fprintf(stderr, " WriteStmt T_WRITE T_STRING Production Rule\n");
						fprintf(stderr, "WriteStmt T_WRITE T_STRING Production Rule\n");
						fprintf(stderr, "	T_STRING: %s\n", $2);
						}

						$$ = ASTCreateNode(A_WRITE);
						$$->name = $2;
						$$->nodeLabel = CreateLabel();
					}
					;

/*	@description:	AssignmentStmt will assign the Var to the value of SimpleExpression
					Added September 27: This production rule creates an A_ASSIGN node and sets the s1 pointer to the
					lefthand side of the statement and the s2 pointer to the righthand side of the statement.
					Added October 26: Two conditionals are checked before the assignment statement is carried out - whether
								or not the lefthand and righthand side of the assignment statement have the same datatype
								or if SubTypes are compatible.
*/
AssignmentStmt		: 	Var T_ASSIGN SimpleExpression ';'
					{	if(mydebug) fprintf(stderr, " AssignmentStmt Production Rule\n");

						if ($1 -> datatype != $3 -> datatype){
							yyerror("Type mismatch in assignment statement.");
							exit(1);
						}

						if ($1->value == -1 || $3->value == -1){
							yyerror("Misuse of subtypes in either the left or right hand side of the equation.");
							exit(1);
						}

						$$ = ASTCreateNode(A_ASSIGN);
						$$ -> s1 = $1;
						$$ -> s2 = $3;
						$$ -> datatype = $1 -> datatype;
						$$ -> symbol = Insert(CreateTemp(), $$ -> datatype, SYM_SCALAR, level, 1, offset++);
					}
					;

/*	@description:	ContinueStmt will continue the iterative loop if found.
					Added September 27: This production rule creates an A_CONT node.
*/
ContinueStmt		:	T_CONT ';'
					{	if(mydebug) fprintf(stderr, " ContinueStmt Production Rule\n");
						$$ = ASTCreateNode(A_CONT);
					}
					;

/*	@description:	BreakStmt will end the iterative loop if found.
					Added September 27: This production rule creates an A_BREAK node.
*/
BreakStmt			:	T_BREAK ';'
					{	if(mydebug) fprintf(stderr, " BreakStmt Production Rule\n");
						$$ = ASTCreateNode(A_BREAK);
					}
					;

/*	@description:	Expression is defined by a SimpleExpression. */
Expression			:	SimpleExpression
					{	if(mydebug) fprintf(stderr, " Expression Production Rule\n");
						$$ = $1;
					}
					;

/*	@description:	Var defines what is a variable - either T_ID or T_ID array.
					Added September 27: This production rule creates an A_VAR node and sets the name and if it is an array,
					sets the array parameters to the s1 pointer.
					Added October 23: Each Var's name is first searched to see if it is already in the symbol table - if not,
									an error is thrown and the program is exited. The value of the node is also set to 0
									if the SubType of the node is a scalar and -1 if it was declared as an array.
*/
Var					:	T_ID
					{	if(mydebug){
							fprintf(stderr, " Var T_ID Production Rule\n");
							fprintf(stderr, "Var T_ID Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", $1);
						}

						//if not in symbol table, barf because in order to use, it must be defined
						//if it's the wrong subt-type, barf
						struct SymbTab * p;
						p = Search($1, level, 1);

						if (p == NULL){
							yyerror($1);
							yyerror("Variable used but not defined.");
							exit(1);
						}

						$$ = ASTCreateNode(A_VAR);
						$$ -> name = $1;
						$$ -> symbol = p;
						$$ -> value = 0;
						$$ -> datatype = p -> Declared_Type;

						if (p->SubType == SYM_ARRAY){
							$$ -> value = -1;
						}
					}

					|	T_ID '[' Expression ']'
					{	if(mydebug) {
							fprintf(stderr, " Var T_ID [Expression] Production Rule\n");
					 		fprintf(stderr, "Var T_ID [Expression] Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", $1);
						}

						struct SymbTab * p;
						p = Search($1, level, 1);

						if (p == NULL){
							yyerror($1);
							yyerror("Variable used but not defined.");
							exit(1);
						}

						if ($3 -> datatype != A_INTTYPE){
							yyerror("Type missmatch used for array length ");
							exit(1);
						}

						$$ = ASTCreateNode(A_VAR);
						$$ -> name = $1;
						$$ -> s1 = $3;
						$$ -> symbol = p;
						$$ -> datatype = p -> Declared_Type;

					}
					;

/*	@description:	SimpleExpression will either call an AdditiveExpression or a Relop Rule on Expression passed into it.
					It is a left-recursive call to account for the mathematics it must do and the compound compilations that
					could occur. For example: (2-3)-7 is different from 2-(3-7), where the parenthesis represent how the
					order of operations will occur depending on what type of recursion is happening on the expression.
					Added September 27: This production rule either directs the parser to the AdditiveExpression production rule
					or creates an A_EXPR node that holds a relative operator in its operator property and an AdditiveExpression in
					its s2 pointer.
					Added October 20: Checks to make sure that the expression is properly evaluated have been added. If either $1
									or $3 are arrays, we check to make sure that the index is within the size of what was
									declared. If it is not within the size of what was declared, then an error is thrown.
									Functions are also not allowed to be used in the SimpleExpression rule. The returned
									value of SimpleExpression node's datatype is set to a boolean value.
*/
SimpleExpression	:	AdditiveExpression
					{	if(mydebug) fprintf(stderr, " SimpleExpression AdditiveExpression Production Rule\n");
						$$ = $1;
					}

					|	SimpleExpression Relop AdditiveExpression
					{	if(mydebug) fprintf(stderr, " SimpleExpression Relop AdditiveExpression Production Rule\n");

						if ($1->datatype == A_VOIDTYPE){
							yyerror("Type missmatch in expression. Cannot assign to void type variable.");
							exit(1);
						}//endif

						if( $1->datatype != $3->datatype){
							yyerror("Type missmatch in expression");
							exit(1);
						}//endif

						if (var_check($1) == 0 || var_check($3) == 0){
							yyerror("Input variable is invalid.");
							exit(1);
						}

						$$ = ASTCreateNode(A_EXPR);
						$$ -> s1 = $1;
						$$ -> s2 = $3;
						$$ -> operator = $2;
						$$ -> datatype = A_BOOLEANTYPE;
						$$ -> symbol = Insert(CreateTemp(), $$ -> datatype, SYM_SCALAR, level, 1, offset++);
					}
					;


/*	@description:	Relop is defined by the relative operator tokens.
					Added September 27: All operator tokens are paired with their enumerated values from the ast.h file.
*/
Relop				: 	T_LE
					{	if(mydebug) fprintf(stderr, " Relop T_LE Production Rule\n");
						$$ = A_LE;
					}

					|	T_GR
					{	if(mydebug) fprintf(stderr, " Relop T_GR Production Rule\n");
						$$ = A_GR;
					}

					|	T_LEEQU
					{	if(mydebug) fprintf(stderr, " Relop T_LEEQU Production Rule\n");
						$$ = A_LEEQU;
					}

					|	T_GREQU
					{	if(mydebug) fprintf(stderr, " Relop T_GREQU Production Rule\n");
						$$ =A_GREQU;
					}

					|	T_NEQU
					{	if(mydebug) fprintf(stderr, " Relop T_NEQU Production Rule\n");
						$$ = A_NEQU;
					}

					|	T_EQUAL
					{	if(mydebug) fprintf(stderr, " Relop T_EQUAL Production Rule\n");
						$$ = A_EQUAL;
					}
					;

/*	@description:	AdditiveExpression will either call Term or Addop on Term passed into it. It is a left-recursive
					call to account for the way the order of operations must work in mathematics. For example:
					(2-3)-7 is different from 2-(3-7), where the parenthesis represent how the order of operations occur
					depending on what type of recursion occurs.
					Added September 27: This production rule either directs the parser to the Term production rule or creates an
									A_EXPR node. The node will store the operator in the expression and Term in its s2 pointer.
					Added October 23: Several checks have been added to the production rule. If an array is used in either
									$1 or $3 of the production rule, we make sure the index presented is valid in comparison to the size
									of the array. Functions are not allowed to be used in this expression as a valid $1 or $3
									value. The datatypes are also checked to ensure that there is compatibility - if not,
									an error is thrown and the program exited.
*/
AdditiveExpression	:	Term
					{	if(mydebug) fprintf(stderr, " AdditiveExpression Term Production Rule\n");
						$$ = $1;
					}

					|	AdditiveExpression Addop Term
					{	if(mydebug) fprintf(stderr, " AdditiveExpression Addop Term Production Rule\n");
						if( $1->datatype != $3->datatype){
							yyerror("Type missmatch in expression");
							exit(1);
						}//endif

						if (var_check($1) == 0 || var_check($3) == 0){
							yyerror("Input variable is invalid in additive expression.");
							exit(1);
						}

						$$ = ASTCreateNode(A_EXPR);
						$$ -> s1 = $1;
						$$ -> s2 = $3;
						$$ -> operator = $2;
						$$ -> datatype = $1 -> datatype;
						$$ -> symbol = Insert(CreateTemp(), $$ -> datatype, SYM_SCALAR, level, 1, offset++);
					}
					;

/*	@description:	Addop is triggered by either an addition or subtraction operator.
					Added September 27: All token operators are paired with their equivalent enumerated datatype in the
					ast.h file.
*/
Addop				:	T_PLUS
					{	if(mydebug) fprintf(stderr, " Addop T_PLUS Production Rule\n");
						$$ = A_PLUS;
					}

					|	T_MINUS
					{	if(mydebug) fprintf(stderr, " Addop T_MINUS Production Rule\n");
						$$ = A_MINUS;
					}
					;

/*	@description:	Term is defined as either a Factor or a Multop on the Factor. It is a left-recursive function to
					account for the way the order of operations must work on division. For example: (5/10)/2 is different from
					5/(10/2), where the parenthesis represent how the order of operations occur depending on what type of
					recursion occurs.
					Added September 27: This production rule either directs the parser to the Factor production rule or creates
										an A_EXPR node. The node will store the operator and the factor in its s1 pointer.
					Added October 23: Several checks have been added to the production rule. If an array is used in either
									$1 or $3 of the production rule, we make sure the index presented is valid in comparison to the size
									of the array. Functions are not allowed to be used in this expression as a valid $1 or $3
									value. The datatypes are also checked to ensure that there is compatibility - if not,
									an error is thrown and the program exited.
*/
Term				:	Factor
					{	if(mydebug) fprintf(stderr, " Term Factor Production Rule\n");
						$$ = $1;
					}

					|	Term Multop Factor
					{	if(mydebug) fprintf(stderr, " Term Multop Factor Production Rule\n");

						if ($1 -> datatype != $3 -> datatype){
							yyerror("Type missmatch in expression");
							exit(1);
						}//endif

						if (var_check($1) == 0 || var_check($3) == 0){
							yyerror($1);
							yyerror("Input variable is invalid in term.");
							exit(1);
						}

						$$ = ASTCreateNode(A_EXPR);
						$$ -> s1 = $1;
						$$ -> s2 = $3;
						$$ -> operator = $2;
						$$ -> datatype = $1 -> datatype;
						$$ -> symbol = Insert(CreateTemp(), $$ -> datatype, SYM_SCALAR, level, 1, offset++);
					}
					;

/*	@description:	Multop is triggered by one of the following tokens.
					Added September 27: All operator tokens are paired with their equivalent enumerated datatype in
					the ast.h file.
*/
Multop				:	T_MUL
					{	if(mydebug) fprintf(stderr, " Multop T_MUL Production Rule\n");
						$$ = A_MUL;
					}

					|	T_DIV
					{	if(mydebug) fprintf(stderr, " Multop T_DIV Production Rule\n");
						$$ = A_DIV;
					}

					|	T_OR
					{	if(mydebug) fprintf(stderr, " Multop T_OR Production Rule\n");
						$$ = A_OR;
					}
					|	T_AND
					{	if(mydebug) fprintf(stderr, " Multop T_AND Production Rule\n");
						$$ = A_AND;
					}
					;

/*	@description:	Factor is defined and triggered by one of the following tokens.
					Added September 27: This production rule will either direct the parser to the Expression or Var production
					rules, create an A_NUM node, an A_FACTOR or an A_EXPR node. The A_NUM node will hold the value of the factor in it.
					The A_EXPR node will hold the operator A_NOT in it. The A_FACTOR node will hold either the true or false
					values in it.
					Added October 23: The datatypes are added to production rules T_TRUE, T_FALSE, and T_NOT. For T_NOT,
									a check for datatype compatability is added. If an item follows not has a datatype
									other than boolean, an error is thrown and the program is exited.
*/
Factor				:	'(' Expression ')'
					{	if(mydebug) fprintf(stderr, " Factor (Expression) Production Rule\n");
						$$ = $2;
					}

					|	T_NUM
					{	if(mydebug) fprintf(stderr, " Factor T_NUM Production Rule\n");
						$$ = ASTCreateNode(A_NUM);
						$$ -> value = $1;
						$$ -> datatype = A_INTTYPE;
					}

					|	Var
					{	if(mydebug) fprintf(stderr, " Factor Var Production Rule\n");
						$$ = $1;
					}

					|	Call
					{	if(mydebug) fprintf(stderr, " Factor Call Production Rule\n");
						$$ = $1;
					}

					| 	T_TRUE
					{	if(mydebug) fprintf(stderr, " Factor T_TRUE Production Rule\n");
						$$ = ASTCreateNode(A_FACTOR);
						$$ -> name = "TRUE";
						$$ -> value = 1;
						$$ -> datatype = A_BOOLEANTYPE;
					}

					|	T_FALSE
					{	if(mydebug) fprintf(stderr, " Factor T_FALSE Production Rule\n");
						$$ = ASTCreateNode(A_FACTOR);
						$$ -> name = "FALSE";
						$$ -> value = 0;
						$$ -> datatype = A_BOOLEANTYPE;
					}

					|	T_NOT Factor
					{	if(mydebug) fprintf(stderr, " Factor T_NOT Factor Production Rule\n");
						if ($2 -> datatype != A_BOOLEANTYPE){
							yyerror("NOT operator expects boolean.");
							exit(1);
						}

						// if (var_check($2) == 0){
						// 	yyerror($1);
						// 	yyerror("Input variable is invalid in NOT operator.");
						// 	exit(1);
						// }

						$$ = ASTCreateNode(A_EXPR);
						$$ -> operator = A_NOT;
						$$ -> s1 = $2;
						$$ -> datatype = $2 -> datatype;

					}
					;

/*	@description:	Call production rule will call a function with the actual arguments per the
					specifications of the parameters set.
					Added October 1: This production rule will create an A_CALL node that holds the name
					of the function being called and the arguments stored in the s1 pointer.
					Added October 23: Several checks have been added to the production rule before a node is created. First,
									we search the symbol table to see if the function has been declared. If it has not, an
									error is thrown and the program is exited. If it has been declared, but it is not
									a function, an error is thrown. If the function in the symbol table requires parameters
									but we have not inputted any, an error is thrown. check_params was added to ensure that
									the datatype of each variable called in the actual parameters matches what is allowed
									from the formal parameters.
*/
Call				:	T_ID '(' Args ')'
					{	if(mydebug) {
							fprintf(stderr, " Call Production Rule\n");
							fprintf(stderr, "Call Production Rule\n");
							fprintf(stderr, "	T_ID: %s\n", $1);
						}

						//check if in symbtable
						struct SymbTab *p;

						p = Search($1, 0, 0);
						if (p == NULL){
							yyerror($1);
							yyerror("Function not declared in symbol table. ");
							exit(1);
						}

						//if symbol is not a function
						if (p-> SubType == SYM_FUNCTION || p-> SubType == SYM_FUNCTION_PROTO){}
						else{
							yyerror($1);
							yyerror("Function name is not defined as a function. ");
							exit(1);
						}

						//if no parameters were passed and $3 is NULL
						//Cannot pass $3 into check_params
						if (($3 == NULL) && (p->fparms->s2 != NULL)){
							yyerror($1);
							yyerror("Incorrect amount of parameters used.");
							exit(1);
						}

						//check to see if actual and formal parameters match in length and type
						if (check_params(p->fparms, $3) == 0){
							yyerror($1);
							yyerror("Parameter usage incorrect. ");
							exit(1);
						}

						$$ = ASTCreateNode(A_CALL);
						$$ -> name = $1;
						$$ -> s1 = $3;
						$$ -> symbol = p;
						$$ -> datatype = $$ -> symbol -> Declared_Type;

					}
					;

/*	@description:	Args represents either an empty argument or a list of actual arguments.
					Added October 1: This production rule will be set to NULL or create an A_ARGS node that
					will store an argument list in its s1 pointer.
					Changed October 23: A_ARGS is no longer a node, instead this production rule points directly to
										either a null node or the ArgList.
*/
Args				:	/*empty*/
					{	if(mydebug) fprintf(stderr, " Args Production Rule\n");
						$$ = NULL;
					}

					|	ArgList
					{	if(mydebug) fprintf(stderr, " Args ArgList Production Rule\n");
						$$ = $1;
					}
					;

/*	@description:	ArgList can comprise of one expression or many. It is a right-recursive function.
					Added October 1: This production rule will create an A_ARGLIST node that will store either
					one argument or a list of arguments in its s1 and s2 pointers.
					Changed October 23: The A_ARGLIST node no longer exists as an option, instead this node now directly references
										the expression node. If there is a list, it is a list of expression nodes.
*/
ArgList				:	Expression
					{	if(mydebug) fprintf(stderr, " ArgList Expression Production Rule\n");
							$$ = ASTCreateNode(A_ARGLIST);
							$$ -> s1 = $1;
							$$ -> name = $1 -> name;
							$$ -> value = $1 -> value;
							$$ -> datatype = $1 -> datatype;
							$$->symbol = Insert(CreateTemp(), $$->datatype, SYM_SCALAR, level, 1, offset++);

					}

					|	Expression ',' ArgList
					{	if(mydebug) fprintf(stderr, " ArgList Expression, ArgList Production Rule\n");

							$$ = ASTCreateNode(A_ARGLIST);
							$$ -> s1 = $1;
							$$ -> s2 = $3;
							$$ -> name = $1 -> name;
							$$ -> value = $1 -> value;
							$$ -> datatype = $1 -> datatype;
							$$->symbol = Insert(CreateTemp(), $$->datatype, SYM_SCALAR, level, 1, offset++);
					}
					;

%%	/* end of rules, start of program */

/*  * @precondition: None
    * @postcondition: None
    * @param: argc - number of arguments given to program
	* @param: argv - name of arguments given to program
	* @return: integer
    * @description: This runs the program*/
int main(int argc, char* argv[])
{ //start main function

	FILE *fp = NULL;
	char s[100];

	for(int i = 1; i < argc; i++){
		if (strcmp("-d", argv[i]) == 0){
			mydebug = 1;
		}//endif

		if (strcmp("-o", argv[i]) == 0){
			//assume that argv[i+1] is on the line & we will not check
			strcpy(s, argv[i+1]);
			strcat(s, ".asm");

			//now you have the file name
			if (mydebug) printf("Opening file %s\n", s);

			fp = fopen(s, "w");
			if (fp == NULL){
				printf("Unablt to open %s\n", s);
				exit(1);
			}//endif
		}//endif stcmp -o
	}//end for loop

	if (fp == NULL){
		printf("No file name given, must use -o option.\n");
		exit(1);
	}//endif

	yyparse();

	if (mydebug) Display();
	if (mydebug) ASTprint(0, program);

	EMIT(program, fp);

} //end main function
