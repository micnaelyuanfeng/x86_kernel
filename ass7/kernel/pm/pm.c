#include "types.h"
#include "color.h"
#include "display.h"
#include "pm.h"

extern uint32_t bios_mem_segs;
extern uint32_t bios_mem_num;

mem_seg_desc_t mem_seg_descriptor[NUM_OF_MEM_SEG];

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

	printk_color(black, green, "This is Memory Segment Info\n");
	for(uint8_t i = 0; i < bios_mem_num; i++)
	{
		printk_hex((mem_seg_descriptor + i)->mem_addr_low);
		printk(" ");
		printk_hex((mem_seg_descriptor + i)->mem_addr_high);
		printk(" ");
		printk_hex((mem_seg_descriptor + i)->mem_len_low);
		printk(" ");
		printk_hex((mem_seg_descriptor + i)->mem_len_high);
		printk(" ");
		printk_hex((mem_seg_descriptor + i)->mem_type);
		printk("\n");
	}
}