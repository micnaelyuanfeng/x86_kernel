#ifndef _IDT_H_
#define _IDT_H_

struct idt_entry;

#define NUM_OF_INT_HANDLER			256

#define SET_IDT_GATE(a, x, y, z, w) ({	\
	((a)[(x)]).base_low   = (y) & 0xFFFF;	\
	((a)[(x)]).base_high  = ((y) >> 16) & 0xFFFF;	\
	((a)[(x)]).selector   = (z);	\
	((a)[(x)]).attr       = (w);	\
	((a)[(x)]).always0    = 0;	\
})

typedef struct idt_entry
{
	uint16_t base_low;
	uint16_t selector;
	uint8_t  always0;
	uint8_t  attr;
	uint16_t base_high;
}__attribute__((packed))idt_entry_t;

typedef struct idt_ptr
{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed))idt_ptr_t;

typedef struct proc_context
{
	uint32_t ds;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t intr_vector;
	uint32_t err_code;
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t usresp;
	uint32_t ss;
} __attribute__((packed))proc_ctx_t;

typedef void (*interrupt_handler_t)(proc_ctx_t* this_context);


void init_idt_table(void);

void register_interrupt_handler(uint8_t intr_vector, interrupt_handler_t this_handler);

void isr_handler_dispatcher(proc_ctx_t* current_ctx);

void irq_handler_dispathcer(proc_ctx_t* current_ctx);

//All ISR (Interrupt Service Routine)
void isr0(void);
void isr1(void);
void isr2(void);
void isr3(void);
void isr4(void);
void isr5(void);
void isr6(void);
void isr7(void);
void isr8(void);
void isr9(void);
void isr10(void);
void isr11(void);
void isr12(void);
void isr13(void);
void isr14(void);
void isr15(void);
void isr16(void);
void isr17(void);
void isr18(void);
void isr19(void);
void isr20(void);
void isr21(void);
void isr22(void);
void isr23(void);
void isr24(void);
void isr25(void);
void isr26(void);
void isr27(void);
void isr28(void);
void isr29(void);
void isr30(void);
void isr31(void);

//void isrX(void);
//void isrX(void);
//void isrX(void);
//void isrX(void);
void isr255(void);


void intr_test(void);

void init_8259A(void); 

#endif