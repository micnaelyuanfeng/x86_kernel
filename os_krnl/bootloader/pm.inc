DA_32		    	equ	    0x4000
DA_LIMIT_4K         equ     0x8000

DA_DPL0             equ     0x0
DA_DPL1             equ     0x20
DA_DPL2             equ     0x40
DA_DPL3             equ     0x60

; Data segment
DA_DR               equ     0x90
DA_DRW              equ     0x92
DA_DRWA             equ     0x93

; Code segment
DA_C                equ     0x98
DA_CR               equ     0x9A
DA_CCO              equ     0x9C
DA_CCOR             equ     0x9E

; Selector
SA_RPL0		    	equ	    0	
SA_RPL1		    	equ	    1	
SA_RPL2		    	equ	    2	
SA_RPL3		    	equ	    3	

SA_TIG		    	equ	    0	
SA_TIL		    	equ	    4

; Gate 
DA_TaskGate			equ	  0x85	; task_gate
DA_386TSS			equ	  0x89	; TSS
DA_386CGate			equ	  0x8C	; 386 call_gate
DA_386IGate			equ	  0x8E	; 386 interrupt
DA_386TGate			equ	  0x8F	; 386 trap

%macro Descriptor 3
	dw	%2 & 0FFFFh				            ; limit
	dw	%1 & 0FFFFh				            ; Base_addr_lo
	db	(%1 >> 16) & 0FFh		            ; Base_addr_mi
	dw	((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)	; arrtbutes and limit
	db	(%1 >> 24) & 0FFh			        ;Base_addr_hi
%endmacro ; 8 bytes total

%macro Gate 4
	dw	(%2 & 0FFFFh)				        ; offset_lo
	dw	%1					                ; selector
	dw	(%3 & 1Fh) | ((%4 << 8) & 0FF00h)	; attribute
	dw	((%2 >> 16) & 0FFFFh)			    ; offset_hi
%endmacro ; 8 bytes total
