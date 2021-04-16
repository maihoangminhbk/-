.data
A:  .word 1,4,5,-6,2,7,-11,12
Aend: .word
.text
main:
	la 	$a0, A  		# a0 = dia chi A[0]
	la	$a1, Aend
	addi	$t0,$a0,4 		# $t0 = dia chi A[i+1] , tao mang con
	j  insertion_sort  		# nhay xuong nhan sap xep
after_sort:
	li	$v0, 10			#exit
	syscall
end_main:
insertion_sort:
	add 	$v0,$t0,0 #$v0 = dia chi A[i+1]
loop:
	lw 	$s0,0($v0)# $s0 = A[1]
	lw  	$s1,-4($v0)  # $s1 = A[0]
	slt 	$t1,$s0,$s1 # A[1] < A[0]
	beq $t1,$zero,continue # if ( A[1] > A[0] ), swap
	sw  $s0,-4($v0)  # A[0] = $s0
	sw  $s1,0($v0) # A[1] = $s1
	addi $v0,$v0,-4 # $v0 = dia chi A[i-1]
	beq $v0,$a0,continue #Tiep tuc 
	j loop
continue:
	addi $t0,$t0,4  # tang so phan tu cua mang con va so sanh tiep
	beq $t0,$a1,end_sort
	j insertion_sort
end_sort: j after_sort