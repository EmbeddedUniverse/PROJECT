	.def _autoCorrASM

_autoCorrASM
	.asmfunc

	MV A4,A0
	MPYI 2,B4,A1
	NOP 8
	SUB A1,1,A1

	MVK 0,B0 				; COUNTER
loopNbrResult:



	ADD B0,1,B0
	CMPLTU B0,A1,A2			; SUBSTACTING FOR loopNbrResult
	[A2] B loopNbrResult 	; IF A1 NONZERO KEEP LOOPING
	NOP 5

	B B3
	NOP 5
	.endasmfunc
