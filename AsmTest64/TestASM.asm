include TestASM.inc

; parameter RCX, RDX, R8, R9
; Must be preserved by callee : RBX, RBP, RDI, RSI, R12, R13, R14, R15 , XMM6-XMM15, YMM6-YMM15


.data


.code
;int Add_ASM(int a, int b)
Add_ASM	PROC
	
	mov [rsp+8],rcx
	mov [rsp+16],rdx
	add rcx,rdx
	mov rax,rcx
	ret
Add_ASM	ENDP

; int Add_ASM_RBP(int a, int b)
Add_ASM_RBP	PROC	a:QWORD, b:QWORD
	LOCAL c:QWORD
	LOCAL d:QWORD
	LOCAL e:QWORD
	LOCAL f:QWORD


	mov a,rcx	;mov [rbp+16],rcx
	mov b,rdx	;mov [rbp+24],rdx
	add rcx,rdx
	mov c, rcx	; mov [rbp-8],rcx
	mov rax,c
	ret
Add_ASM_RBP	ENDP

; unsigned int GetMaxValue_ASM64_CMOV(const unsigned int* puiValueList, unsigned int uiCount)
GetMaxValue_ASM64_CMOV PROC   puiValueList:QWORD, uiCount:DWORD
    push rsi

    mov puiValueList,rcx	;mov [rbp+16],rcx
	mov uiCount,edx     	;mov [rbp+24],rdx

    mov rsi, rcx
    mov rcx, rdx

	xor rax,rax
    xor rdx,rdx
    
lb_cmp_and_swap:
    mov edx,dword ptr[rsi]
    cmp rdx,rax
    cmovae rax,rdx

    add rsi,4
    dec rcx
    jnz lb_cmp_and_swap

    pop rsi
    
    ret
GetMaxValue_ASM64_CMOV ENDP

; unsigned int GetMaxValue_ASM64_XOR(const unsigned int* puiValueList, unsigned int uiCount)
GetMaxValue_ASM64_XOR PROC   puiValueList:QWORD, uiCount:DWORD
    push rsi

    mov puiValueList,rcx	;mov [rbp+16],rcx
	mov uiCount,edx     	;mov [rbp+24],rdx

    mov rsi, rcx
    mov rcx, rdx

	xor rax,rax
    xor rdx,rdx
    xor r8,r8

lb_cmp_and_swap:
    xor r8,r8
    mov edx,dword ptr[rsi]

    cmp rax,rdx
    sbb r8, 0      ; 0 - (0 + CF)
    
    mov r9, rax     ; r8 = and mask
    xor r9, rdx     ; r9 = xor mask(rax ^ rdx)
    and r9, r8

    xor rax,r9
    ;xor rdx,r9

    add rsi,4
    dec rcx
    jnz lb_cmp_and_swap

    pop rsi
    
    ret
GetMaxValue_ASM64_XOR ENDP
end

