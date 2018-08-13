#include "types.h"
#include "list.h"
#include "pmm.h"


/************************************************
	Free Block Combine Functions
	-> both neighbours are allocated
	-> next block is free = can be combined
	-> prev block is free = can be combined
	-> next and prev block are free and can be combined
*************************************************/
uint32_t stack_heap[MAX_NUM_PAGES]; //suport 20MB heap
uint32_t heap_stack;

extern uint32_t num_of_page;
extern uint32_t pm_start_addr;
extern uint32_t pm_end_addr;

// Hyposize largest allocated chunk size is 4096 Bytes
// Then 2^12, 12 free list for different size of chunks
list_entry_t* free_list[NUM_OF_LIST];

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
		
		size += GET_SIZE(HDRP(PREV_BLKP(this_block)));
		
		PUT(FTRP(this_block), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(this_block)), PACK(size, 0));
		
		return (PREV_BLKP(this_block));
	}
	else
	{
		list_entry_t* next = (list_entry_t*)NEXT_BLKP(this_block);
		list_entry_t* prev = (list_entry_t*)PREV_BLKP(this_block);
		
		size += GET_SIZE(HDRP(NEXT_BLKP(this_block)));
		size += GET_SIZE(HDRP(PREV_BLKP(this_block)));;
		
		list_del(prev);
		list_del(next);
		
		PUT(HDRP(PREV_BLKP(this_block)), PACK(size, 0));
		PUT(HDRP(NEXT_BLKP(this_block)), PACK(size, 0));
	
		return (PREV_BLKP(this_block));
	}
}

void* extend_heap()
{
	uint32_t* block_ptr = NULL;
	uint32_t size      = PAGE_SIZE;

	if(heap_stack == num_of_page)
	{
		return NULL;
	}
	else
	{
		block_ptr = (uint32_t*)stack_heap[heap_stack];
	
		PUT(HDRP(block_ptr), PACK(size, 0));
		PUT(FTRP(block_ptr), PACK(size, 0));
		
		heap_stack++;
		
		list_add((list_entry_t*)block_ptr);
			
		return (void*)block_ptr;
	}
}

bool mm_init()
{
	heap_stack = 0;
	
	//Initialize free list
	for(uint8_t i = 0; i < 12; i++)
	{
		free_list[i] = NULL;
	}

	for(uint8_t i = 0; i < MAX_NUM_PAGES; i++)
	{
		stack_heap[i] = -1;
	}
	//alignment is 4K page
#define ALIGNMENT			4096
	pm_start_addr = (pm_start_addr + (ALIGNMENT-1)) & ~(ALIGNMENT-1);
	pm_end_addr   = pm_end_addr & ~(ALIGNMENT-1);
	
	num_of_page   = (pm_end_addr - pm_start_addr) / ALIGNMENT;
	
	for(uint8_t i = 0; i < num_of_page; i++)
	{
		stack_heap[i] = pm_start_addr + i * (ALIGNMENT - 1);
	}
	
	return 0;
}

/*
	This Function find the corresponding link list
	for the input size
*/
void* mm_search_fit(uint32_t alloc_size)
{
	if(alloc_size == 0)
	{
		return NULL;
	}
	
	uint32_t free_list_index   = list_hash(alloc_size);
	uint8_t  list_index        = 0;
	uint32_t block_size        = 0;
	uint32_t remaining_size	   = 0;
	
	list_entry_t* list_head_ptr = NULL;
	list_entry_t* traverse_ptr  = NULL;
	list_entry_t* list_elem_ptr = NULL;
	list_entry_t* remaining_ptr = NULL;
	list_entry_t* alloc_ptr     = NULL;
	
	//Using First Fit Algo
	for(list_index = free_list_index; list_index < 12; list_index++)
	{
		if(free_list[list_index] != NULL)
		{
			//Get List Head
			list_elem_ptr = free_list[list_index];
			
			//Get Block Information
			block_size = GET_SIZE(HDRP(list_elem_ptr));
			
			uint32_t left_size = 0;
			//Situation 1
			if(block_size >= alloc_size && (block_size - alloc_size) >= MIN_SIZE)
			{
				list_del(list_elem_ptr);

				remaining_size = block_size - alloc_size;
				
				alloc_ptr = (uint8_t*)list_elem_ptr + remaining_size;
				
				//Mark allocated chunck information
				PUT(HDRP(alloc_ptr), PACK(alloc_size, 1));
				PUT(FTRP(alloc_ptr), PACK(alloc_size, 1));
				
				//Mark leftover chunk information
				PUT(HDRP(list_elem_ptr), PACK(remaining_size, 1));
				PUT(FTRP(list_elem_ptr), PACK(remaining_size, 1));
			
				//Update the free list if remaining size is not zero
				if(remaining_size > 0)
				{
					list_index    = list_hash(remaining_size);
					list_head_ptr = free_list[list_index];

					//if no posiition is NULL
					if(list_head_ptr != NULL)
					{
						//list_add_before(list_elem_ptr);
						list_add(list_elem_ptr);
					}
					else if(list_head_ptr == NULL)
					{
						free_list[list_index] = list_elem_ptr;
						list_init(list_elem_ptr);
					}
					else
					{
						list_add(list_elem_ptr);
					}
					
					return (void*)alloc_ptr;
				}
			}
			else if(block_size >= alloc_size && (block_size - alloc_size) < MIN_SIZE)//Reamaining size is not enough to make a min_size
			{
				list_del(list_elem_ptr);
				
				return (void*)list_elem_ptr;
			}
		
			//if first fit does not work, then traverse
			list_elem_ptr = list_elem_ptr->next;
		
			for(; list_elem_ptr != NULL; list_elem_ptr = list_elem_ptr->next)
			{
				block_size = GET_SIZE(HDRP(list_elem_ptr));
				
				if(block_size >= alloc_size && (block_size - alloc_size) >= MIN_SIZE)
				{
					list_del(list_elem_ptr);
					
					left_size = block_size - alloc_size;
					
					alloc_ptr = (uint8_t*)list_elem_ptr + left_size;
					
					//Update Footer and Header
					PUT(HDRP(alloc_ptr), PACK(alloc_size, 1));
					PUT(FTRP(alloc_ptr), PACK(alloc_size, 1));
					
					PUT(HDRP(list_elem_ptr),  PACK(left_size, 0));
					PUT(FTRP(list_elem_ptr),  PACK(left_size, 0));
					
					if(left_size > 0)
					{
						list_index      = list_hash(left_size);
						list_head_ptr   = free_list[list_index];
						
						if(list_head_ptr != NULL)
						{
							list_add_before(list_head_ptr, list_elem_ptr);
						}
						else if(list_head_ptr == NULL)
						{
							free_list[list_index] = list_elem_ptr;
							list_init(list_elem_ptr);
						}
						else
						{
							list_add(list_elem_ptr);
						}
						
						return (void*)alloc_ptr;
					}
				}
				else
				{
					list_del(list_elem_ptr);
					
					return (void*)list_elem_ptr;
				}
			}
		}
	}
}

void  mm_mark_used(void* this_chunck, uint32_t alloc_size)
{
	uint32_t block_size = GET_SIZE(HDRP(this_chunck));
	
	PUT(HDRP(this_chunck), PACK(block_size, 1));
    PUT(FTRP(this_chunck), PACK(block_size, 1));
}	

void mm_free(void* this_chunk)
{
	if(this_chunk == NULL)
	{
		return;
	}
	
	uint32_t size = GET_SIZE(HDRP(this_chunk));
	
	PUT(HDRP(this_chunk), PACK(size,0));
    PUT(FTRP(this_chunk), PACK(size,0));
	
	//list_entry_t* 
}

void* mm_malloc(uint32_t size)
{
	uint32_t alloc_size  = 0;
	uint32_t extend_size = 0;
	uint8_t* alloc_ptr   = NULL;

	if(size <= 0)
	{
		return NULL;
	}
	
	if(size <= 512)
	{
		size = next_pow_of_2(size);
	}
	
	if(size <= DWSIZE)
	{
		alloc_size = DWSIZE + OVERHEAD;
	}
	else
	{
		alloc_size = DWSIZE * ((size + (OVERHEAD) + (DWSIZE-1))/ DWSIZE);
	}
	
	if((alloc_ptr = mm_search_fit(alloc_size)) != NULL)
	{
		mm_mark_used(alloc_ptr, alloc_size);
		
		return alloc_ptr;
	}
	
	extend_size = alloc_size;
	
	if((alloc_ptr = extend_heap(extend_size / DWSIZE)) == NULL)
	{
		return NULL;
	}
	
	mm_mark_used(alloc_ptr, alloc_size);
	
	return alloc_ptr;
		
}

