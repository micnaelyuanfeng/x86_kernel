#include "types.h"
#include "lib.h"
#include "display.h"
#include "idt.h"

/******************************************************

IDT Operation FLow

Interrupt Service Routine = Push Error Code or Not
Interrupt Entry = Context Save
Interrupt Dispatcher functions
Interrupt ISR register

2 types of Interrupt = Error Code / No Error Code

1. Define ALL Interrupt Entry functions for the 2 type = ISR codes = 256
		All ISR function will call Interrupt Entry = Common One to save current process context
2. Define Interrupt Common Function
		Interrupt Entry Functions call isr_handler_dispatcher
3. Interrupt Handler Dispatcher
		call Correspodining ISR base on Interrupt Vector
4. Each ISR will be impelemented and call Interrput register


Call Stack

External Hardware Interrupt -> Hardwares parse the vector number

call idt[vector]


idt[vector] = {
	Interrupt Entry Function  = push Int number/Error Code
	Interrupt Comman Function = Context Save 
	Interrupt Dispatcher
	Registered Interrupt Handler
}

*******************************************************/


idt_entry_t idt_vectors[NUM_OF_INT_HANDLER];

idt_ptr_t idtPtr;

interrupt_handler_t[NUM_OF_INT_HANDLER];

void isr_handler_dispatcher(proc_ctx_t* current_ctx)
{
	if(idt_vectors[current_ctx->intr_vector])
	{
		idt_vectors[current_ctx->intr_vector](current_ctx);
	}
	else
	{
		printk_color(black, red, "Unhandled Interrupt: %d\n", current_ctx->intr_vector);
	}
}

void init_idt_table(void)
{
	idtPtr.base   = idt_vectors;
	idtPtr.limit  = sizeof(idt_entry_t) * NUM_OF_INT_HANDLER - 1

	SET_IDT_GATE(0, (uint32_t)isr0, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr1, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr2, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr3, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr4, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr5, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr6, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr7, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr8, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr9, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr10, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr11, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr12, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr13, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr14, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr15, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr16, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr17, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr18, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr19, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr20, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr21, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr22, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr23, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr24, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr25, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr26, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr27, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr28, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr29, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr30, 0x08, 0x8E);
	SET_IDT_GATE(0, (uint32_t)isr31, 0x08, 0x8E);

	SET_IDT_GATE(0, (uint32_t)isr255, 0x08, 0x8E);
}

void intr_test(void)
{
	__asm__ volatile ("int 0x0");
	__asm__ volatile ("int 0x1");
	__asm__ volatile ("int 0x2");
	__asm__ volatile ("int 0x3");
	__asm__ volatile ("int 0x4");
	__asm__ volatile ("int 0x5");
	__asm__ volatile ("int 0x6");
}