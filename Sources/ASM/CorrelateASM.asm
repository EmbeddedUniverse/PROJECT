	.def _CorrelateASM

_CorrelateASM
	.asmfunc

	; A4 = ResultCorr
	; B4 = signalX
	; A6 = signalRef
	; B6 = nbDataSignalX
	; A8 = nbDataSignalRef

;*************************************************
; Mémoriser le contenu de AMR
			MVC	AMR, B14
			STW	B14,*--B15
;*************************************************

	MVK 0,A0 				; A0 = COUNTER i
	MVK 0,A1				; A1 = COUNTER j
	ADD B6,A8,A3			; A2 = LENGTH CORRELATION
	SUB A8,1,A10			; nbDataSignalRef-1
	SUB B6,1,A11			; nbDataSignalX-1
	MV B4,A15

LoopIndiceCorr:

	CMPLT A0, A8, A2; if (i >= nbDataSignalRef)
	[!A2] SUB A0,A10,A1  ; start = i - (nbDataSignalRef-1);
	[!A2] MV A0,A12     ; end=i

	CMPLT A0, B6, B0; if (i >= nbDataSignalX)
	[!B0] MV A11,A12; end=i     ; end = (nbDataSignalX-1);

LoopCorrelation:

	SUB A0,A10,A13
	SUB A1,A13,A13

	ADDAW A15,A1,A15 		; GET signalX[j]
	;LDW *A15,A15
	NOP 4
	ADDAW A6,A13,A6 		; GET signalRef[j-(i -(nbDataSignalRef-1))]
	;LDW *A6,A6
	NOP 4

	MPY A15,A6,A14;signalX[j] *signalRef[j-(i -(nbDataSignalRef-1))]
	NOP 1

	ADDAW A5,A0,A5 		; GET signalX[j]
	;LDW *A5,A5
	NOP 4
	ADD A5,A14,A5;ResultCorrTemp[i] = ResultCorrTemp[i] + signalX[j] *signalRef[j-(i -(nbDataSignalRef-1))];

	CMPGT A1, A12, A2
	ADD A1,1,A1
	[!A2] B LoopCorrelation
	NOP 5

	CMPLT A3,A0,A2
	ADD A0,1,A0
	[!A2] B LoopIndiceCorr
	NOP 5

	MV A5,A4;VÉRIFIER

; Rétablir le contenu de AMR
			LDW	*B15++,B5
			NOP	5
			MVC	B5,AMR

	B B3
	NOP 5
	.endasmfunc
