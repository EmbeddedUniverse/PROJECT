	.def _autocorrelateASM

_autocorrelateASM
	.asmfunc

	; A4 = INPUT VALUES
	; A6 = OUTPUT VALUES
	; B4 = NBR OF INPUT VALUES
	; B6 = NBR OF OUTPUT VALUES

;*************************************************
; Mémoriser le contenu de AMR
			MVC	AMR, B14
			STW	B14,*--B15
;*************************************************

	MVK 0,A0 				; A0 = COUNTER i
	MVKL 0,A10				; A10 = RESULT
	MVKH 0,A10



loopNbrResult:

;K = A7
	SUB B4,1,A9 ; (N-1)
	SUB A0,A9,A7

;START = A8 = N
	CMPGT A7,0,A2
	[A2] MV A7,A8
	[!A2] MVK 0,A8

;END = A9
	CMPLT A7,0,A2
	[A2] MV A0,A9
	ADD A9,1,A9

loopResult:
;***********************************************************************;
;SIGNAL(N)
	SUB A8,A7,A11			;GET N-K
||	ADDAW A4,A8,A12 		; GET ADDRESS SIGNAL(N)
	LDW *A12,A12			;GET VALUE SIGNAL(N)
;SIGNAL(N-K)
	ADDAW A4,A11,A11; 		;GET ADRESS SIGNAL(N-K)
	LDW *A11,A11			;GET VALUE SIGNAL(N)
	NOP 4
;SIGNAL(N)*SIGNAL(N-K)
	MPYSP A11,A12,A11		;MULTIPLY SIGNAL(N) AND SIGNAL(N-K)
	NOP 3
;ADD TO ANSWER
	ADDSP A10,A11,A10		;Result+=SIGNAL(N)*SIGNAL(N-K)
;***********************************************************************;
||	ADD A8,1,A8				; N++
	CMPLT A8,A9,A2			; ADD FOR loopNbrResult
	[A2] B loopResult
	NOP 5

;STORING IN OUTPUT VECTOR
	ADDAW A6,A0,A11			; FIND ADRESS OF OUTPUT
	STW A10,*A11			; STORE A10 IN OUTPUT VECTOR

||	MVKL 0,A10				; RESET A10
	MVKH 0,A10


	ADD A0,1,A0				; I++
	CMPLT A0,B6,A2			; ADD FOR loopNbrResult
	[A2] B loopNbrResult 	; IF BO UNDER A1 KEEP LOOPING
	NOP 5

; Rétablir le contenu de AMR
			LDW	*B15++,B5
			NOP	5
			MVC	B5,AMR

	B B3
	NOP 5
	.endasmfunc
