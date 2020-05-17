.code
	NtCreateThreadEx PROC
		mov r10, rcx
		mov eax, 00A5h   //kindly check this according to your PC(win7, 8 or 10)
		syscall
		ret
	NtCreateThreadEx ENDP
end
