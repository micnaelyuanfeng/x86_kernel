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
#define PAGE_SIZE					4096
#define PAGE_NUM_MAX				2048  //32MB HEAP MAX SUPPORT

void     mem_seg_desc_copy(void);
void     mem_seg_desc_display(void);
void     init_phy_memoey(void);

uint32_t phy_mem_allocate_page(void);
bool     phy_mem_free_page(void);

void*    kmalloc(uint32_t size);
bool     kfree(void* p_addr);


//void
//void
//void

#endif