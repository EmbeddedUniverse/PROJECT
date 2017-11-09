	.def _add2CircularBuffer

_add2CircularBuffer
	.asmfunc
;float* add2CircularBuffer(float *adress, int sizeBuffer, float value,);
; A4 = adress to add to
; B4 = size buffer
; A6 = value to add

; Mémoriser le contenu de AMR
	MVC	AMR, B14
||	MVKL 32,A5
	STW	B14,*--B15
||	MVKH 32,A5

;*************************************************
; SET CIRCULAR BUFFER
	LMBD 	1, B4, B5
	SUB 	A5, B5, B5
	SHL		B5, 16, B5

||	MVKL 	0X0001,A5
	OR 		B5,A5,B5
	MVC 	B5,AMR

;*************************************************
;STORE VALUE
	STW 	A6,*A4++

; Rétablir le contenu de AMR
			LDW	*B15++,B5
			NOP	5
			MVC	B5,AMR

	B B3
	NOP 5
	.endasmfunc
