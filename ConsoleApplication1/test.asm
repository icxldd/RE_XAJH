.code

extern test1:proc

add1 proc

	add rcx,rdx
	mov rax,rcx
	ret

add1 endp




test2 proc

	mov ecx,0ch;
	mov edx,5;
	call test1;
	ret;

test2 endp

end