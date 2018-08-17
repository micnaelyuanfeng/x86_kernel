#include "types.h"
#include "color.h"
#include "display.h"
#include "pm.h"
#include "pmm.h"

extern uint32_t bios_mem_segs;
extern uint32_t bios_mem_num;

mem_seg_desc_t mem_seg_descriptor[NUM_OF_MEM_SEG];

static uint32_t mem_heap[PAGE_NUM_MAX];
static uint32_t mem_heap_top;
static uint32_t physical_page_count;

extern uint32_t stack_heap[MAX_NUM_PAGES]; //suport 20MB heap
extern uint32_t heap_stack;
extern uint32_t num_of_page;

void mem_seg_desc_copy(void)
{
	for(uint8_t i = 0; i < bios_mem_num; i++)
	{
		mem_seg_descriptor[i].mem_addr_low  = ((mem_seg_desc_t*)bios_mem_segs + i)->mem_addr_low;
		mem_seg_descriptor[i].mem_addr_high = ((mem_seg_desc_t*)bios_mem_segs + i)->mem_addr_high;
		mem_seg_descriptor[i].mem_len_low   = ((mem_seg_desc_t*)bios_mem_segs + i)->mem_len_low;
		mem_seg_descriptor[i].mem_len_high  = ((mem_seg_desc_t*)bios_mem_segs + i)->mem_len_high;
		mem_seg_descriptor[i].mem_type      = ((mem_seg_desc_t*)bios_mem_segs + i)->mem_type;
	}

	
}

void mem_seg_desc_display()
{
	printk_color(black, green, "This is Memory Segment Info\n");
	
	for(uint8_t i = 0; i < bios_mem_num; i++)
	{
		printk_color(black, cyan, "Base Address = 0x%x%08x, Length = 0x%x%08x, Type = 0x%x\n",
					 mem_seg_descriptor[i].mem_addr_high, mem_seg_descriptor[i].mem_addr_low,
					 mem_seg_descriptor[i].mem_len_high, mem_seg_descriptor[i].mem_len_low,
					 mem_seg_descriptor[i].mem_type);	
	}
}

void init_phy_memoey(void)
{
	//get avaiable physical memory and calculate number of pages
	//Hard code choose seg = 3 as heap
	uint32_t kernel_starting_address = 0x030400;

	//choose segment 4 as heap source
	//uint32_t heap_start_address = 0x100000;
	//uint32_t heap_length        = 0x1EF0000;
	//uint32_t heap_end           = heap_start_address + heap_length;

	//physical_page_count         = 0;
	heap_stack                = 0;

	uint32_t heap_start_addr  = 0;
	uint32_t heap_length      = 0;
	uint32_t heap_end         = 0;

	physical_page_count       = 0;

	for(uint32_t i = 0; i < bios_mem_num; i++)
	{
		if(mem_seg_descriptor[i].mem_type == 0x1 &&
		   mem_seg_descriptor[i].mem_len_low >= 0x1000000)
		   {
			   heap_start_addr = mem_seg_descriptor[i].mem_addr_low;
			   heap_start_addr = heap_start_addr + (PAGE_SIZE - 1) & ~(PAGE_SIZE - 1); 

			   heap_length     = mem_seg_descriptor[i].mem_len_low;
		   }
	}

	//Need to make Page Size Aligned

	for(uint32_t i = 0; i < heap_length; i = i + PAGE_SIZE)
	{
		physical_page_count++;

	}

	for(uint32_t i = heap_start_addr; i < (heap_length + heap_start_addr); i += PAGE_SIZE)
	{
		stack_heap[heap_stack] = i;
		heap_stack++;
	}

	//reset heap_stack
	heap_stack = 0;
	num_of_page = physical_page_count;

	printk_color(light_grey, red, "Heap Start Address   = 0x%x\n", heap_start_addr);
	printk_color(light_grey, red, "Num Of Free Pages    = 0x%x\n", physical_page_count);
	printk_color(light_grey, red, "Kernel Start Address = 0x%x\n", kernel_starting_address);

	//printk_color(light_grey, red, "Head dump");

	//for(uint32_t i = 0; i < physical_page_count; i++)
	//{
	//	printk("Heap Address = 0x%x\n", stack_heap[i]);
	//}

}