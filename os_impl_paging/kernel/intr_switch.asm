[SECTION .isr_entry]
; Interrupt Service Rotine
; These handlers are used for
; Software Interrupts

extern isr_handler_dispatcher

global isr_ctx_save

isr_ctx_save:
	pusha
	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	push esp
	call isr_handler_dispatcher
	add esp, 0x4

	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	mov ss, bx

	popa
	add esp, 0x8        ;esp points to EIP for CPU to push back automatically
	iret

%macro ISR_NOERRCODE	1
global isr%1

isr%1:
	cli
	push 0				 ;Push Invalid error code = consistent
	push %1
	jmp  isr_ctx_save

%endmacro

%macro ISR_ERRCODE    1
global isr%1

isr%1:
	cli
	push %1               ;CPU will push err code
	jmp  isr_ctx_save
%endmacro

ISR_NOERRCODE 		0
ISR_NOERRCODE 		1
ISR_NOERRCODE 		2
ISR_NOERRCODE 		3
ISR_NOERRCODE 		4
ISR_NOERRCODE 		5
ISR_NOERRCODE 		6
ISR_NOERRCODE 		7
ISR_ERRCODE 		8
ISR_NOERRCODE 		9
ISR_ERRCODE 		10
ISR_ERRCODE 		11
ISR_ERRCODE 		12
ISR_ERRCODE 		13
ISR_ERRCODE 		14
ISR_NOERRCODE 		15
ISR_NOERRCODE 		16
ISR_ERRCODE 		17
ISR_NOERRCODE 		18
ISR_NOERRCODE 		19
ISR_NOERRCODE 		20
ISR_NOERRCODE 		21
ISR_NOERRCODE 		22
ISR_NOERRCODE 		23
ISR_NOERRCODE 		24
ISR_NOERRCODE 		25
ISR_NOERRCODE 		26
ISR_NOERRCODE 		27
ISR_NOERRCODE 		28
ISR_NOERRCODE 		29
ISR_NOERRCODE 		30
ISR_NOERRCODE 		31

ISR_NOERRCODE 		255


; Interrupt Request Rotine
; These handlers are used for
; HardwareInterrupts
[SECTION .irq_entry]
extern irq_handler_dispathcer

global irq_ctx_save

irq_ctx_save:
	pusha
	mov ax, ds
	push eax

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	push esp
	call irq_handler_dispathcer
	add esp, 0x4

	pop ebx
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx
	mov ss, bx

	popa
	add esp, 0x8
	iretd

%macro IRQ 2

global	irq%1

irq%1:
	cli

	push byte 0
	push byte %2
	jmp  irq_ctx_save

%endmacro

IRQ 0,	32
IRQ 1,	33
IRQ 2,	34
IRQ 3,	35
IRQ 4,	36
IRQ 5,	37
IRQ 6,	38
IRQ 7,	39
IRQ 8,	40
IRQ 9,	41
IRQ 10,	42
IRQ 11,	43
IRQ 12,	44
IRQ 13,	45
IRQ 14,	46
IRQ 15,	47