	.def _autoCorrASM

_autoCorrASM
	.asmfunc

	; A4 = INPUT VALUES
	; A6 = OUTPUT VALUES
	; B4 = NBR OF INPUT VALUES


; Mémoriser le contenu de AMR
			MVC	AMR, B14
			STW	B14,*--B15


	MPYI 2,B4,A1			; GET 2N
	MVK 0,A0 				; A0 = COUNTER i
	MVKL 0,A10				; A10 = RESULT
	MVKH 0,A10
	MV	A4,B6
	MV B3,B13
	NOP 5
	SUB A1,1,A1				; GET RESULT LENGTH A1= RESULT LENGTH



loopNbrResult:

;K = B1
	SUB B4,1,B3 ; (N-1)
	SUB A0,B3,B1

;START = B2 = N
	CMPGT B1,0,A2
	[A2] MV B1,B2
	[!A2] MVK 0,B2

;END = B3
	CMPLT B1,0,A2
	[A2] MV A0,B3
	ADD B3,1,B3

loopResult:
;***********************************************************************;
;SIGNAL(N)
	ADDAW B6,B2,B12 		; GET ADDRESS SIGNAL(N)
	LDW *B12,B12			;GET VALUE SIGNAL(N)
	NOP 4
;SIGNAL(N-K)
	SUB B2,B1,B8			;GET N-K
	ADDAW B6,B8,B11; 		;GET ADRESS SIGNAL(N-K)
	LDW *B11,B11			;GET VALUE SIGNAL(N)
	NOP 4
;SIGNAL(N)*SIGNAL(N-K)
	MPYSP B11,B12,B11		;MULTIPLY SIGNAL(N) AND SIGNAL(N-K)
	NOP 3
;ADD TO ANSWER
	ADDSP A10,B11,A10		;Result+=SIGNAL(N)*SIGNAL(N-K)
;***********************************************************************;
	ADD B2,1,B2				; N++
	CMPLT B2,B3,B0			; ADD FOR loopNbrResult
	[B0] B loopResult
	NOP 5

;STORING IN OUTPUT VECTOR
	ADDAW A6,A0,A11			; FIND ADRESS OF OUTPUT
	STW A10,*A11			; STORE A10 IN OUTPUT VECTOR

	MVKL 0,A10				; RESET A10
	MVKH 0,A10


	ADD A0,1,A0				; I++
	CMPLT A0,A1,A2			; ADD FOR loopNbrResult
	[A2] B loopNbrResult 	; IF BO UNDER A1 KEEP LOOPING
	NOP 5

; Rétablir le contenu de AMR
			LDW	*B15++,B5
			NOP	5
			MVC	B5,AMR

	B B13
	NOP 5
	.endasmfunc
