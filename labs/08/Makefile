proc: proc.c
	gcc proc.c -o proc
	gcc -O3 proc.c -o proc-O3
	objdump -d ./proc > no-opt.log
	objdump -d ./proc-O3 > O3.log
