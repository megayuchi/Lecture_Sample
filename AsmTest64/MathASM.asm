include math_type.inc

; parameter RCX, RDX, R8, R9
; Must be preserved by callee : RBX, RBP, RDI, RSI, R12, R13, R14, R15 , XMM6-XMM15, YMM6-YMM15

.data


.code

;void TransformVector4_VPTR2_ASM(VECTOR4* pv4Dest, const VECTOR4* pv4Src, const MATRIX4* pMat, unsigned int uiCount)
TransformVector4_VPTR2_ASM	PROC pv4Dest:QWORD, pv4Src:QWORD, pMat:QWORD, uiCount:QWORD
	; rcx = pv4Dest
	; rdx = pv4Src
	; r8 = pMat
	; r9 = uiCount

	LOCAL xmm6_backup:XMMWORD	
	LOCAL xmm7_backup:XMMWORD

	; backup xmm6, xmm7
	movaps	xmm6_backup,xmm6
	movaps	xmm7_backup,xmm7

	; if (!uiCount) exit
	test		r9,r9
	jz			lb_exit
	
	; arg home area
	mov		pv4Dest,rcx
	mov		pv4Src,rdx
	mov		pMat,r8
	mov		uiCount,r9
	
	sub		rcx,rdx

	; load matrix
	movups	xmm4,xmmword ptr[r8]	; load matrix 1 line
	movups	xmm5,xmmword ptr[r8+16]	; load matrix 2 line
	movups	xmm6,xmmword ptr[r8+32]	; load matrix 3 line
	movups	xmm7,xmmword ptr[r8+48]	; load matrix 4 line

lb_loop:
	; load (x,y,z,w) from pv4Src
	movups	xmm0,xmmword ptr[rdx]
	
	movaps	xmm1,xmm0		; src vector
	movaps	xmm2,xmm0		; src vector
	shufps	xmm1,xmm1,85	; y y y y
	shufps	xmm2,xmm2,170	; z z z z 
	movaps	xmm3,xmm0		; src vector
	;movaps	xmm0,xmm0		; src vector
	shufps	xmm3,xmm3,255	; w w w w 
	shufps	xmm0,xmm0,0		; x x x x

	mulps	xmm1,xmm5		; yyyy * _m1_0123
	mulps	xmm0,xmm4		; xxxx * _m0_0123
	mulps	xmm2,xmm6		; zzzz * _m2_0123
	addps	xmm0,xmm1		
	mulps	xmm3,xmm7		; wwww * _m3_0123
	addps	xmm2,xmm3
	addps	xmm0,xmm2
	
	

	; write (x,y,z,w) to pv4Dest
	movups	xmmword ptr[rcx+rdx],xmm0
	add		rdx,VECTOR4_SIZE

	dec		r9
	jnz		lb_loop
		
	; restore xmm6
	movaps	xmm7,xmm7_backup
	movaps	xmm6,xmm6_backup

lb_exit:
	ret

TransformVector4_VPTR2_ASM	ENDP
end

