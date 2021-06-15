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
	local		t1:qword	; 16����Ʈ ������� ���߱� ����
		
	; �������� ����
	push		r12
	push		r13
	
	; rbp�������͸� push�����Ƿ� 8����Ʈ �ɼ��� ���� ��ġ���� arg home�����̴�
	mov			qword ptr[rbp+8+8],rcx
	mov			qword ptr[rbp+8+16],rdx
	mov			qword ptr[rbp+8+24],r8
	mov			qword ptr[rbp+8+32],r9
	

	
	mov			r10,r8			; ppVArgList
	mov			r11,r9			; dwVArgNum
	xor			r9,r9
	
	
	
	; ���ڰ� 0���� ����
	or			r11,r11
	jz			lb_return
	
	
	
	; ���ڰ� 1�� �̻��̹Ƿ� ù��° ���� �غ�
	mov			rax,qword ptr[r10]		; ù��° ����
	mov			r8,rax
	add			r10,8
	
	; ���ڰ� 1���ΰ�?
	cmp			r11,1
	jnz			lb_arg_2

	sub			rsp,32	
	call		fscanf
	add			rsp,32
	
	jmp			lb_return
	
	
lb_arg_2:

	; ���ڰ� 2�� �̻��̹Ƿ� �ι�° ���� �غ�
	mov			rax,qword ptr[r10]
	mov			r9,rax
	add			r10,8
	
	; ���ڰ� 2���ΰ�?
	cmp			r11,2
	jnz			lb_arg_3_above
	
	sub			rsp,32
	call		fscanf
	add			rsp,32
	
	jmp			lb_return
	
lb_arg_3_above:
	; �������ڰ� 3���̻�, �׷��ϱ� �� ���ڼ��� 5���̻�
		
	
	sub			r11,2		; �������� ������ ���ڰ���
	
	mov			r12,r11		; copy 
	shl			r12,3		; * 8		; ���ڸ� �����ϱ� ���� ���� ������
	
	mov			t,r12 			; ���߿� ���� ������ ���� ������ ����
	
	sub			rsp,r12		; ���� ������ ���� ���� Ȯ��
	
	
	mov			r13,rsp		; ���ڸ� ����� ��Ʈ�� ����Ʈ

lb_loop:
	mov			rax,qword ptr[r10]		; �������� ������ ù��° ����
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






