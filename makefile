#Trang Tran
#November 8, 2023
#compiler makefile
#--------------------------------------------------------
#	This makefile will compile both compiler_lex.l and compiler_yacc.y files into one compiler
#	executable file. The compiler_yacc.y file is turned into y.tab.c and y.tab.h file. The .h file
#	is not used to compile into an executable, but it will be used in the compiler_lex.l file
# 	as a dependency needed. The compiler_lex.l file is turned into a lex.yy.c file as well and
#	will be used in conjunction with the y.tab.c file to compile into an executable. The ast.h,
#	ast.c, symtable-2.c, and symtable.h files are used in the compiler call to fully build the program.

#--------------------------------------------------------------------------------------------------------
#The default label is compiler and allows us to run compiler.o
default: compiler

#The label compiler depends on the lex.yy.c and y.tab.c files that was previously
#translated using lex and yacc. compiler uses the C-compiler gcc to compile the c code
#in ast.c, ast.h, symtable-2.c, symtable.h, lex.yy.c and y.tab.c into an executable program.
compiler: lex.yy.c y.tab.c ast.c ast.h symtable.c symtable.h emit.h emit.c
	gcc y.tab.c lex.yy.c ast.c symtable.c emit.c -o compiler

#The label y.tab.c depends on the compiler_yacc.y file and will use yacc to
#parse it into a y.tab.c and y.tab.h file.
y.tab.c: compiler_yacc.y
	yacc -dv compiler_yacc.y

#The lex.yy.c label depends on compiler_lex.l file. Using lex, we can parse the
#program and turn it into a .c file.
lex.yy.c: compiler_lex.l
	lex compiler_lex.l

#This clears any information previously compiled or parsed. It allows us to
#start from a clean slate.
clean:
	-rm -f lex.yy.c y.tab.c y.tab.h *.o
