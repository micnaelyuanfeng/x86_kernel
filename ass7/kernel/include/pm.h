#ifndef _PM_H_
#define _PM_H_

typedef struct mem_seg_desc
{
	uint32_t mem_addr_low;
	uint32_t mem_addr_high;
	uint32_t mem_len_low;
	uint32_t mem_len_high;
	uint32_t mem_type;
}__attribute__((packed))mem_seg_desc_t;

#define NUM_OF_MEM_SEG				10

void mem_seg_desc_copy(void);

#endif