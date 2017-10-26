	.def _autoCorrASM

_autoCorrASM
	.asmfunc

	MV A4,A15
	MPYI 2,B4,A1
	NOP 8
	SUB A1,1,A1

	MVK 0,A0 				; COUNTER i

loopNbrResult:
	;K = B1
	SUB B4,1,B3 ; (N-1)
	SUB B0,B3,B1

	;START = B2
	CMPGT B1,0,A2
	[A2] MV B1,B2
	[!A2] MVK 0,B2

	;END = B3
	CMPLT B1,0,A2
	[A2] MV B0,B3
	ADD B3,1,B3

loopResult:


	ADD B2,1,B2
	CMPLT B2,B3,B0			; ADD FOR loopNbrResult
	[B0] B loopResult
	NOP 5


	ADD A0,1,A0
	CMPLT A0,A1,A2			; ADD FOR loopNbrResult
	[A2] B loopNbrResult 	; IF BO UNDER A1 KEEP LOOPING
	NOP 5

	B B3
	NOP 5
	.endasmfunc
