#include "types.h"
#include "color.h"
#include "gdt.h"
#include "idt.h"
#include "lib.h"
#include "pm.h"
#include "display.h"
#include "vga.h"
#include "keyboard.h"
#include "timer.h"
#include "pmm.h"
#include "list.h"
 
#define NUM_OF_GDT_ENTRY				8
#define GDT_SIZE						8

uint32_t         num_of_page;
uint8_t          gdtPtr[NUM_OF_GDT_ENTRY];
gdt_descriptor_t gdt[GDT_SIZE];

void* MemCopy(void* p_dest, void* p_src, int size);

uint8_t cursor_x = 0;
uint8_t cursor_y = 0;

extern list_entry_t* free_list[NUM_OF_LIST];
extern uint32_t bios_mem_segs;
extern uint32_t bios_mem_num;
extern uint32_t heap_stack;


/*
	GDT Solution 1: copy and store current GDT register value to 
					local variuable of kern.bin
	GDT solution 2: create new one GDT in kern.bin
*/ 
    
void cstart()
{ 
	uint32_t gdt_idx = 0;

	for(; gdt_idx < GDT_SIZE; gdt_idx++)
	{
		gdt[gdt_idx].limit_low        = 0xFFFF;
		gdt[gdt_idx].base_low         = 0xFFFF;
		gdt[gdt_idx].base_mid         = 0xFF;
		gdt[gdt_idx].attr     		  = 0xFF;
		gdt[gdt_idx].limit            = 0xFF;
		gdt[gdt_idx].base_high		  = 0xFF;
	}
  
  	//Solituin 1
	//MemCopy(&gdt, (void*)(*((uint32_t*)(&gdtPtr[2]))), *((uint16_t*)(&gdtPtr[0])) + 1);

	//Solution 2
	gdt_descriptor_set(gdt[0], 0 , 	    0,       0);
	gdt_descriptor_set(gdt[1], 0, 		0xFFFFF, DA_CR|DA_32|DA_LIMIT_4K);
	gdt_descriptor_set(gdt[2], 0, 		0xFFFFF, DA_DRW|DA_32|DA_LIMIT_4K);
	gdt_descriptor_set(gdt[3], 0xB8000, 0xFFFF,  DA_DRW|DA_DPL3);

	*((uint32_t*)(&gdtPtr[2])) = (uint32_t)&gdt;
 
	console_clear();

	//printk_color(black, light_green, "This is test string\n");
	mem_seg_desc_copy();

	mem_seg_desc_display();

	init_phy_memoey();

 	init_idt_table();

	init_timer(30000);
	
	init_keyboard();

	//init temp page table
	for(uint32_t i = 0; i )
	{

	}

	//set selector

} 