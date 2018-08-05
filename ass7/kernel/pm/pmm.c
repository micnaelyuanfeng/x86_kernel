#include "pmm.h"


/************************************************
	Free Block Combine Functions
	-> both neighbours are allocated
	-> next block is free = can be combined
	-> prev block is free = can be combined
	-> next and prev block are free and can be combined
*************************************************/
void* coalesce(void* this_block)
{
	uint32_t prev_block_allocated = GET_ALLOC(FTRP(PREV_BLKP(this_block)));
	uint32_t next_block_allocated = GET_ALLOC(HDRP(NEXT_BLKP(this_block)));
	uint32_t size                 = GET_SIZE(HDRP(this_block));

	if(prev_block_allocated && next_block_allocated)
	{
		return this_block;
	}
	else if(prev_block_allocated && !next_block_allocated)
	{
		list_entry_t* next = (list_entry_t*)NEXT_BLKP(this_block);
		list_del(next);
		
		size += GET_SIZE(HDRP(NEXT_BLKP(this_block)));
		
		PUT(HDRP(this_block), PACK(size, 0));
		PUT(FTRP(this_block), PACK(size, 0));
		
		return this_block;
	}
	else if(!prev_block_allocated && next_block_allocated)
	{
		list_entry_t* prev = (list_entry_t*)PREV_BLKP(this_block);
		list_del(prev);
		
		size += GET_SIZE(HDRP(PREV_BLKP)(this_block)));
		
		PUT(FTRP(this_block), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(this_block)), PACK(size, 0));
		
		return (PREV_BLKP(this_block));
	}
	else
	{
		list_entry_t* next = (list_entry_t*)NEXT_BLKP(this_block);
		list_entry_t* prev = (list_entry_t*)PREV_BLKP(this_block);
		
		size += GET_SIZE(HDRP(NEXT_BLKP(this_block)));
		size += GET_SIZE(HDRP(PREV_BLKP)(this_block)));
		
		list_del(prev);
		list_del(next);
		
		PUT(HDRP(PREV_BLKP(this_block)), PACK(size, 0));
		PUT(HDRP(NEXT_BLKP(this_block)), PACK(size, 0));
	
		reuturn (PREV_BLKP(this_block));
	}
}

void* extend_heap()
{
	uint8_t* block_ptr = NULL;
	uint32_t size      = PAGE_SIZE;
	
	block_ptr = heap_stack[heap_index];
	
	PUT(HDRP(block_ptr), PAGE_SIZE(size, 0));
	PUT(FTRP(block_ptr), PAGE_SIZE(size, 0));
	
	list_add(block_ptr);
	
	return (void*)block_ptr;
}

bool mm_init()
{
	//Initialize free list
	for(uint8_t* i = 0; i < 12; i++)
	{
		free_list[i] = NULL;
	}
	
	return 0;
}

