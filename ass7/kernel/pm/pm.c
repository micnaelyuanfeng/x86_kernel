#include "types.h"
#include "color.h"
#include "display.h"
#include "pm.h"

extern uint32_t bios_mem_segs;
extern uint32_t bios_mem_num;

mem_seg_desc_t mem_seg_descriptor[NUM_OF_MEM_SEG];

static uint32_t mem_heap[PAGE_NUM_MAX];
static uint32_t mem_heap_top;
static uint32_t physical_page_count;


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

	printk_color(light_grey, red, "Kernel Start Address = 0x%x\n", kernel_starting_address);

	//choose segment 4 as heap source
	uint32_t heap_start_address = 0x100000;
	uint32_t heap_length        = 0x1EF0000;
	uint32_t heap_end           = heap_start_address + heap_length;

	physical_page_count         = 0;

	for(uint32_t i = 0; i < heap_length; i = i + PAGE_SIZE)
	{

		physical_page_count++;

	}

	printk_color(light_grey, red, "Num Of Free Pages    = 0x%x\n", physical_page_count);

}