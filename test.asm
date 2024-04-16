#  Compilers MIPS code Fall 2023
.data      

_L0: .asciiz        "\nEnter a number :"
_L1: .asciiz        "Fibonacci of "
_L2: .asciiz        " = "
_L3: .asciiz        "\n\n"

.align 2     


.text      

.globl main      

getNum:			# START OF FUNCTION

	subu $a0, $sp, 12		# Adjust the stack for function setup
	sw $sp, ($a0)			# Store old SP
	sw $ra, 4($a0)			# Store current return address (RA)
	move $sp, $a0			# Adjust SP

	li $v0, 4				# Print a string
	la $a0, _L0				# Print a string
	syscall					# Perform write

	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 8			# Emit VAR:: local stack pointer plus offset to get location of local variable
	li $v0, 5				# Read number from input
	syscall					# Reading number
	sw $v0, ($a0)			# Store read input into memory location

	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 8			# Emit VAR:: local stack pointer plus offset to get location of local variable
	lw $a0, ($a0)			# Expression is A_VAR, get value.
	lw $ra 4($sp)			# Return:: Restore old RA
	lw $sp ($sp)			# Return:: Return from function and store SP
	jr $ra					# Return:: return to caller

	li $a0, 0				# RETURN has no specified value set to 0
	lw $ra, 4($sp)			# Restore RA
	lw $sp, ($sp)			# Restore SP
	jr $ra					# Return to caller


fibonacci:			# START OF FUNCTION

	subu $a0, $sp, 40		# Adjust the stack for function setup
	sw $sp, ($a0)			# Store old SP
	sw $ra, 4($a0)			# Store current return address (RA)
	move $sp, $a0			# Adjust SP

	sw $t0, 8($sp)			# Load temp variable int formal parameter
	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 8			# Emit VAR:: local stack pointer plus offset to get location of local variable
	lw $a0, ($a0)			# Expression is A_VAR, get value.
	sw $a0, 12($sp)			# Expr - EQUAL:: stores LHS temporarily
	li $a0 0				# Expression is a constant
	move $a1, $a0			# Expr - EQUAL:: right hand side needs to be a1
	lw $a0, 12($sp)			# Expr - GREQU:: restore LHS from memory
	slt $t2, $a0, $a1		# Expr - EQUAL:: place the greater than value in $t2
	slt $t3, $a1, $a0		# Expr - EQUAL:: compare and save value in $a0
	nor $a0, $t2, $t3		# Expr - EQUAL:: check equality
	andi $a0, 1				# Expr - EQUAL:: store output in $a0

					#IF BODY 
	beq $a0 $0, _L4			# If:: branch to else-body
	li $a0 0				# Expression is a constant
	lw $ra 4($sp)			# Return:: Restore old RA
	lw $sp ($sp)			# Return:: Return from function and store SP
	jr $ra					# Return:: return to caller
	j _L5					# If:: if-body end
_L4:				#ELSE:: label
					#ELSE BODY
_L5:				#END OF IF:: label
	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 8			# Emit VAR:: local stack pointer plus offset to get location of local variable
	lw $a0, ($a0)			# Expression is A_VAR, get value.
	sw $a0, 16($sp)			# Expr - EQUAL:: stores LHS temporarily
	li $a0 1				# Expression is a constant
	move $a1, $a0			# Expr - EQUAL:: right hand side needs to be a1
	lw $a0, 16($sp)			# Expr - GREQU:: restore LHS from memory
	slt $t2, $a0, $a1		# Expr - EQUAL:: place the greater than value in $t2
	slt $t3, $a1, $a0		# Expr - EQUAL:: compare and save value in $a0
	nor $a0, $t2, $t3		# Expr - EQUAL:: check equality
	andi $a0, 1				# Expr - EQUAL:: store output in $a0

					#IF BODY 
	beq $a0 $0, _L6			# If:: branch to else-body
	li $a0 1				# Expression is a constant
	lw $ra 4($sp)			# Return:: Restore old RA
	lw $sp ($sp)			# Return:: Return from function and store SP
	jr $ra					# Return:: return to caller
	j _L7					# If:: if-body end
_L6:				#ELSE:: label
					#ELSE BODY
_L7:				#END OF IF:: label

			#Setting Up Function Call
			#Evaluate Function Parameters
	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 8			# Emit VAR:: local stack pointer plus offset to get location of local variable
	lw $a0, ($a0)			# Expression is A_VAR, get value.
	sw $a0, 20($sp)			# Expr - MINUS:: stores LHS temporarily
	li $a0 1				# Expression is a constant
	move $a1, $a0			# Expr - MINUS:: right hand side needs to be a1
	lw $a0, 20($sp)			# Expr - MINUS:: restore LHS from memory
	sub $a0, $a0, $a1		# Expr - MINUS:: subtract $a1 from $a0, placing back into $a0

	sw $a0, 24($sp)			# Call ARG:: store argument temporarily
	lw $a0, 24($sp)			# ArgList:: Pull out stored argument and place in $a0
	move $t0, $a0			# ArgList:: Move argument from $a0 to temporary register
	jal fibonacci			# Call the function

	sw $a0, 36($sp)			# Expr - PLUS:: stores LHS temporarily

			#Setting Up Function Call
			#Evaluate Function Parameters
	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 8			# Emit VAR:: local stack pointer plus offset to get location of local variable
	lw $a0, ($a0)			# Expression is A_VAR, get value.
	sw $a0, 28($sp)			# Expr - MINUS:: stores LHS temporarily
	li $a0 2				# Expression is a constant
	move $a1, $a0			# Expr - MINUS:: right hand side needs to be a1
	lw $a0, 28($sp)			# Expr - MINUS:: restore LHS from memory
	sub $a0, $a0, $a1		# Expr - MINUS:: subtract $a1 from $a0, placing back into $a0

	sw $a0, 32($sp)			# Call ARG:: store argument temporarily
	lw $a0, 32($sp)			# ArgList:: Pull out stored argument and place in $a0
	move $t0, $a0			# ArgList:: Move argument from $a0 to temporary register
	jal fibonacci			# Call the function

	move $a1, $a0			# Expr - PLUS:: right hand side needs to be a1
	lw $a0, 36($sp)			# Expr - PLUS:: restore LHS from memory
	add $a0, $a0, $a1		# Expr - PLUS:: add $a0 and $a1, placing back into $a0

	lw $ra 4($sp)			# Return:: Restore old RA
	lw $sp ($sp)			# Return:: Return from function and store SP
	jr $ra					# Return:: return to caller

	li $a0, 0				# RETURN has no specified value set to 0
	lw $ra, 4($sp)			# Restore RA
	lw $sp, ($sp)			# Restore SP
	jr $ra					# Return to caller


main:			# START OF FUNCTION

	subu $a0, $sp, 44		# Adjust the stack for function setup
	sw $sp, ($a0)			# Store old SP
	sw $ra, 4($a0)			# Store current return address (RA)
	move $sp, $a0			# Adjust SP


			#Setting Up Function Call
			#Evaluate Function Parameters
	jal getNum			# Call the function

	sw $a0, 32($sp)			# Assign:: Stores RHS temporarily
	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 24			# Emit VAR:: local stack pointer plus offset to get location of local variable
	lw $a1, 32($sp)			# Assign:: Gets RHS temporarily
	sw $a1, ($a0)			# Assign:: Places RHS into memory

	li $v0, 4				# Print a string
	la $a0, _L1				# Print a string
	syscall					# Perform write

	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 24			# Emit VAR:: local stack pointer plus offset to get location of local variable
	lw $a0, ($a0)			# Expression is A_VAR, get value.
	li $v0 1				# Print number
	syscall					# Perform write


			#Setting Up Function Call
			#Evaluate Function Parameters
	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 24			# Emit VAR:: local stack pointer plus offset to get location of local variable
	lw $a0, ($a0)			# Expression is A_VAR, get value.
	sw $a0, 36($sp)			# Call ARG:: store argument temporarily
	lw $a0, 36($sp)			# ArgList:: Pull out stored argument and place in $a0
	move $t0, $a0			# ArgList:: Move argument from $a0 to temporary register
	jal fibonacci			# Call the function

	sw $a0, 40($sp)			# Assign:: Stores RHS temporarily
	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 24			# Emit VAR:: local stack pointer plus offset to get location of local variable
	lw $a1, 40($sp)			# Assign:: Gets RHS temporarily
	sw $a1, ($a0)			# Assign:: Places RHS into memory

	li $v0, 4				# Print a string
	la $a0, _L2				# Print a string
	syscall					# Perform write

	move $a0, $sp			# Emit VAR:: local variable found. Make copy of SP
	addi $a0, $a0 24			# Emit VAR:: local stack pointer plus offset to get location of local variable
	lw $a0, ($a0)			# Expression is A_VAR, get value.
	li $v0 1				# Print number
	syscall					# Perform write

	li $v0, 4				# Print a string
	la $a0, _L3				# Print a string
	syscall					# Perform write


	li $a0, 0				# RETURN has no specified value set to 0
	lw $ra, 4($sp)			# Restore RA
	lw $sp, ($sp)			# Restore SP
	li $v0, 10				# Leave main program
	syscall					# Exiting entire program
