#ifndef _IDT_H_
#define _IDT_H_

typedef struct
{
	uint16_t base_low;
	uint16_t selector;
	uint8_t  always0;
	uint8_t  flags;
	uint16_t base_high;
}__attribute__((packed))idt_entry_t;

typedef struct 
{ 
	uint16_t limit;
	uint32_t base_addr;
}__attribute__((packed))idtPtr;


#define IDT_SIZE			256

void init_8259A(void); 

#endif