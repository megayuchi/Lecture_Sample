include TestASM.inc

; parameter RCX, RDX, R8, R9
; Must be preserved by callee : RBX, RBP, RDI, RSI, R12, R13, R14, and R15 

.data
g_string  db 'Hello Assembly', 0

.code
; INT64 FillString(char* pDest, INT64 Count)
FillString_ASM	PROC	pDest:QWORD, Count:QWORD
	LOCAL qwLen:QWORD	; rbp-8
	LOCAL t0:QWORD		; rbp-16
	LOCAL t1:QWORD		; rbp-24
	LOCAL t2:QWORD		; rbp-32
	LOCAL szTxt[32]:BYTE	; rbp-64

	mov pDest,rcx	; set pDest(rbp+16)
	mov Count,rdx	; set Count (rbp+24)

	push rsi
	push rdi
	
	lea rsi,g_string	; g_string's ptr

	; rax = strlen(g_string)
	mov rcx,rsi	; strlen(rcx)
	sub rsp,32	; alloc arg home area
	call strlen	; call strlen
	add rsp,32	; dealloc arg home area
	
	; if (rax > Count-1) rax = Count-1 
	mov rdx,Count
	dec rdx
	cmp rax,rdx
	jng lb_call_memcpy

	; rax = Count-1
	mov rax,rdx	

lb_call_memcpy:
	mov qwLen,rax	; qwLen = strlen(g_string)

	; call memcpy
	mov r8,rax		; memcpy(?,?,r8)
	mov rdx,rsi		; memcpy(?,rdx,r8)
	mov rcx,pDest	; memcpy(rcx,rdx,r8)
	sub rsp,32		; alloc arg home area
	call memcpy		; call memcpy
	add rsp,32		; dealloc arg home area

	
	; call MessageBox(hWnd,lpText,lpCation,uType)
	xor rcx,rcx			; hWnd
	lea rdx,g_string	; lpText
	mov r8,rdx			; lpCation
	xor r9,r9			; uTupe
	sub rsp,32			; alloc arg home area
	call MessageBoxA	; call MessageBox, Check RSP is aligned to 16 bytes before calling the function! 
	add rsp,32			; dealloc arg home area

	mov rax,qwLen

	pop rdi
	pop rsi

	ret
FillString_ASM	ENDP

end

