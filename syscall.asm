.code
	NtCreateThreadEx PROC
		mov r10, rcx
		mov eax, 00A5h
		syscall
		ret
	NtCreateThreadEx ENDP
end
