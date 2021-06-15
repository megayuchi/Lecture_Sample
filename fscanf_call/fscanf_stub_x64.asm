include fscanf_stub_x64.inc


.code


; RCX, RDX, R8, R9
; varg_stub_x64(FILE* fp,char* szFormat,void** ppVArgList,QWORD qwVArgNum)
; valatile		RAX, RCX, RDX, R8, R9, R10, R11 
; non volatile	RBX, RBP, RDI, RSI, R12, R13, R14, and R15 

fscanf_stub_x64 PROC
	; rcx = FP
	; rdx = szFormat
	; r8 = ppVArgList
	; r9 = qwVArgNum
	

	local		t:qword
	local		t1:qword	; 16바이트 얼라인을 맞추기 위해
		
	; 레지스터 저장
	push		r12
	push		r13
	
	; rbp레지스터를 push했으므로 8바이트 옵셋을 더한 위치부터 arg home영역이다
	mov			qword ptr[rbp+8+8],rcx
	mov			qword ptr[rbp+8+16],rdx
	mov			qword ptr[rbp+8+24],r8
	mov			qword ptr[rbp+8+32],r9
	

	
	mov			r10,r8			; ppVArgList
	mov			r11,r9			; dwVArgNum
	xor			r9,r9
	
	
	
	; 인자가 0개면 리턴
	or			r11,r11
	jz			lb_return
	
	
	
	; 인자가 1개 이상이므로 첫번째 인자 준비
	mov			rax,qword ptr[r10]		; 첫번째 인자
	mov			r8,rax
	add			r10,8
	
	; 인자가 1개인가?
	cmp			r11,1
	jnz			lb_arg_2

	sub			rsp,32	
	call		fscanf
	add			rsp,32
	
	jmp			lb_return
	
	
lb_arg_2:

	; 인자가 2개 이상이므로 두번째 인자 준비
	mov			rax,qword ptr[r10]
	mov			r9,rax
	add			r10,8
	
	; 인자가 2개인가?
	cmp			r11,2
	jnz			lb_arg_3_above
	
	sub			rsp,32
	call		fscanf
	add			rsp,32
	
	jmp			lb_return
	
lb_arg_3_above:
	; 가변인자가 3개이상, 그러니까 총 인자수는 5개이상
		
	
	sub			r11,2		; 스택으로 전달할 인자갯수
	
	mov			r12,r11		; copy 
	shl			r12,3		; * 8		; 인자를 전달하기 위한 스택 사이즈
	
	mov			t,r12 			; 나중에 스택 복구를 위해 사이즈 저장
	
	sub			rsp,r12		; 인자 전달을 위한 공간 확보
	
	
	mov			r13,rsp		; 인자를 써넣을 엔트리 포인트

lb_loop:
	mov			rax,qword ptr[r10]		; 스택으로 전달할 첫번째 인자
	mov			qword ptr[r13],rax
	add			r10,8
	add			r13,8
	dec			r11
	jnz			lb_loop

	sub			rsp,32
	call		fscanf
	add			rsp,32
	add			rsp,t
		

lb_return:
	pop			r13
	pop			r12
		
	ret

	
	
fscanf_stub_x64 ENDP


testfunc PROC x:qword , y:qword , c:real4 , d:real4

	mov			qword ptr[rbp+8+8],rcx
	mov			qword ptr[rbp+8+16],rdx
	movss		dword ptr[rbp+8+24],xmm2
	movss		dword ptr[rbp+8+32],xmm3
	
	
	mov		rax,x
	mov		rdx,y
	add		rax,rdx
	
	movaps		xmm0,c
	movaps		xmm1,d
	addps		xmm0,xmm1
	
	ret	
	
	
	


testfunc endp




end






