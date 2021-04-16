.data
	prompt: .asciiz "Enter an integer, -1 to stop: "
.text
	la $a0, prompt
	li $v0, 4
	syscall
	move $a0, $a1
	li $v0, 5
	syscall
	move $s0, $v0
	start_loop:
		sne $t1, $s0, -1
		beqz $t1, end_loop
		#code block
		la $a0, prompt
		li $v0, 4
		syscall
		move $a0, $a1
		li $v0, 5
		syscall
		move $s0, $v0
		b start_loop
	end_loop:
