#ifndef _PMM_H_
#define _PMM_H_

#define DWSIZE				sizeof(void*)
#define OVERHEAD			DWSIZE
#define PAGESIZE			4096

#define MAX(x, y)			((x) >= (y) ? (x) : (y))
#define PACK(size, alloc)	((size) | alloc)

#define GET(p)				(*(uint32_t*)(p))
#define PUT(p, val)     	(*(uint32_t*)(p) = (val))

#define GET_SIZE(p)			(GET(p) & ~(DWSIZE - 1))
#define GET_ALLOC(p)    	(GET(p) & 0x1)

#define ALIGN_4K(p)     	((p) + (PAGESIZE - 1) & ~(PAGESIZE - 1))

//Get Block Information From Header or Footer
#define HDRP(bp)			((char *)(bp))
#define FTRP(bp)			((char *)(bp) + GET_SIZE(HDRP(bp)))

//Get Block Ptr of Next block or Previous block 
#define NEXT_BLKP(bp)		((char*)(bp) + GET_SIZE((char*)(bp)))
#define PREV_BLKP(bp)   	((char*)(bp) - GET_SIZE((char*)(bp)))

struct block_info
{
	uint32_t allocated : 1;
	uint32_t size      : 31;
};

struct list_entry 
{
	uint32_t          info; //first DWORD is info, only for info protection
    struct list_entry *prev;
    struct list_entry *next;
};

typedef struct list_entry list_entry_t;


// Hyposize largest allocated chunk size is 4096 Bytes
// Then 2^12, 12 free list for different size of chunks
list_entry_t* free_list[12];

//List Management Functions
uint32 list_hash(uint32_t size);

#endif
