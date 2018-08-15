#ifndef _PMM_H_
#define _PMM_H_

struct block_info;

#define DWSIZE				4
#define OVERHEAD			DWSIZE
#define PAGE_SIZE			4096
#define MAX_NUM_PAGES		1024 * 8   //4KB * 1024 = 4MB * 8 = 32MB heap size
#define MIN_SIZE            2 * DWSIZE

#define MAX(x, y)			((x) >= (y) ? (x) : (y))
#define PACK(size, alloc)	((size) | alloc)

#define GET(p)				(*(uint32_t*)(p))
#define PUT(p, val)     	(*(uint32_t*)(p) = (val))

#define GET_SIZE(p)			(GET(p) & ~(DWSIZE - 1))
#define GET_ALLOC(p)    	(GET(p) & 0x1)

#define ALIGN_4K(p)     	((p) + (PAGESIZE - 1) & ~(PAGESIZE - 1))

//Get Block Information From Header or Footer
#define HDRP(bp)			((char *)(bp) - DWSIZE)
#define FTRP(bp)			((char *)(bp) + GET_SIZE(HDRP(bp)) - DWSIZE)

//Get Block Ptr of Next block or Previous block 
#define NEXT_BLKP(bp)		((char*)(bp) + GET_SIZE((char*)(bp)))
#define PREV_BLKP(bp)   	((char*)(bp) - GET_SIZE((char*)(bp)))

struct block_info
{
	uint32_t allocated : 1;
	uint32_t size      : 31;
};

void* mm_malloc(uint32_t size);
void  mm_free(void* this_chunk);

void  free_list_dump(void);

#endif
