#include "types.h"
#include "lib.h"
#include "color.h"
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

interrupt_handler_t interrupt_handler[NUM_OF_INT_HANDLER];

void isr_handler_dispatcher(proc_ctx_t* current_ctx)
{

  printk_color(black, red, "ISR Interrupt Dispatcher\n");

  if(interrupt_handler[current_ctx->intr_vector])
  {
    printk_color(black, red, "ISR Handle Interrupt: %d\n", current_ctx->intr_vector);

    interrupt_handler[current_ctx->intr_vector](current_ctx);
  }
  else
  {
    printk_color(black, red, "Unhandled Interrupt: %d\n", current_ctx->intr_vector);
  }
}

void irq_handler_dispathcer(proc_ctx_t* current_ctx)
{
  //printk_color(black, red, "IRQ Interrupt Dispatcher\r");

  if(current_ctx->intr_vector >= 40)
  {
    outb(0xA0, 0x20);
  }

  outb(0x20, 0x20);

  if(interrupt_handler[current_ctx->intr_vector])
  {
    //printk_color(black, red, "IRQ Handle Interrupt: %d\r", current_ctx->intr_vector);

    interrupt_handler[current_ctx->intr_vector](current_ctx);
  }
  else
  {
    //printk("Unhandled Interrupt: %d\r", current_ctx->intr_vector);
  }

}

void init_idt_table(void)
{
  bzero((void *)&interrupt_handler, sizeof(interrupt_handler[0]) * NUM_OF_INT_HANDLER);

  idtPtr.base   = (uint32_t)idt_vectors;
  idtPtr.limit  = (uint16_t)(sizeof(idt_vectors) * NUM_OF_INT_HANDLER - 1);

  SET_IDT_GATE(idt_vectors, 0, (uint32_t)isr0, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 1, (uint32_t)isr1, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 2, (uint32_t)isr2, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 3, (uint32_t)isr3, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 4, (uint32_t)isr4, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 5, (uint32_t)isr5, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 6, (uint32_t)isr6, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 7, (uint32_t)isr7, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 8, (uint32_t)isr8, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 9, (uint32_t)isr9, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 10, (uint32_t)isr10, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 11, (uint32_t)isr11, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 12, (uint32_t)isr12, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 13, (uint32_t)isr13, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 14, (uint32_t)isr14, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 15, (uint32_t)isr15, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 16, (uint32_t)isr16, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 17, (uint32_t)isr17, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 18, (uint32_t)isr18, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 19, (uint32_t)isr19, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 20, (uint32_t)isr20, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 21, (uint32_t)isr21, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 22, (uint32_t)isr22, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 23, (uint32_t)isr23, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 24, (uint32_t)isr24, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 25, (uint32_t)isr25, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 26, (uint32_t)isr26, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 27, (uint32_t)isr27, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 28, (uint32_t)isr28, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 29, (uint32_t)isr29, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 30, (uint32_t)isr30, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 31, (uint32_t)isr31, 0x08, 0x8E);

  SET_IDT_GATE(idt_vectors, 255, (uint32_t)isr255, 0x08, 0x8E);


  SET_IDT_GATE(idt_vectors, 32, (uint32_t)irq0, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 33, (uint32_t)irq1, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 34, (uint32_t)irq2, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 35, (uint32_t)irq3, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 36, (uint32_t)irq4, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 37, (uint32_t)irq5, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 38, (uint32_t)irq6, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 39, (uint32_t)irq7, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 40, (uint32_t)irq8, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 41, (uint32_t)irq9, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 42, (uint32_t)irq10, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 43, (uint32_t)irq11, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 44, (uint32_t)irq12, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 45, (uint32_t)irq13, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 46, (uint32_t)irq14, 0x08, 0x8E);
  SET_IDT_GATE(idt_vectors, 47, (uint32_t)irq15, 0x08, 0x8E);

  //printk("IDT TABLE Inititlization Done\n");

  init_8259A();

}

void register_interrupt_handler(uint8_t n, interrupt_handler_t h)
{
  interrupt_handler[n] = h;
}

void init_8259A(void)
{

  outb (0x20, 0x11);
  outb (0xA0, 0x11);

  outb (0x21, 0x20);
  outb (0xA1, 0x28);

  outb (0x21, 0x04);
  outb (0xA1, 0x02);

  outb(0x21, 0x01);
  outb(0xA1, 0x01);

  outb(0x21, 0x0);
  outb(0xA1, 0x0);

  printk("8259A Chip Inititlization Done\n");

} 

void intr_test(void)
{
  printk("Interrupt Test\n");
  //__asm__ volatile ("int %0\n" : "N"(0x3));
  //__asm__ volatile ("int %0\n" : "N"(0x4));
}