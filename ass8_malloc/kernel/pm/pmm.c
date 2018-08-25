#include "types.h"
#include "list.h"
#include "color.h"
#include "display.h"
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
	uint32_t prev_block_allocated = GET_ALLOC(NEXT_BLKP(HDRP(this_block)));
	uint32_t next_block_allocated = GET_ALLOC(PREV_BLKP(HDRP(this_block)));
	uint32_t size                 = GET_SIZE(HDRP(this_block));

	uint8_t fisrt_alloc           = ((struct block_info*)HDRP(this_block))->first;

	PUT(HDRP(this_block), PACK(size, 0));
    PUT(FTRP(HDRP(this_block)), PACK(size, 0));

	//printk("this block size: %d\n", size);
	//printk("next block size: %d 0x%x\n", GET_SIZE(NEXT_BLKP(HDRP(this_block))), NEXT_BLKP(HDRP(this_block)));
	//printk("previous block size: %d 0x%x %d %d\n", GET_SIZE(PREV_BLKP(HDRP(this_block))), PREV_BLKP(HDRP(this_block)), ((struct block_info*)(PREV_BLKP(HDRP(this_block))))->first, ((struct block_info*)(PREV_BLKP(HDRP(this_block))))->allocated);

	uint8_t  list_index           = -1;


	if(prev_block_allocated && next_block_allocated)
	{
		printk("Colaesce hit 1\n");

		return this_block;
	}
	else if(prev_block_allocated && !next_block_allocated)
	{
		printk("Colaesce hit 2\n");

		
		list_entry_t* prev = (list_entry_t*)PREV_BLKP(this_block);

		uint32_t      prev_blk_size = GET_SIZE(HDRP(prev));

		list_index                  = list_hash(prev_blk_size); 
		
		list_del(prev, list_index);
		
		size += GET_SIZE(HDRP(PREV_BLKP(this_block)));
		
		PUT(FTRP(this_block), PACK(size, 0));
		PUT(HDRP(PREV_BLKP(this_block)), PACK(size, 0));
		
		return (PREV_BLKP(this_block));

	}
	else if(!prev_block_allocated && next_block_allocated)
	{
		printk("Colaesce hit 3\n");

		if(fisrt_alloc != 1)
		{
			list_entry_t* next          = (list_entry_t*)NEXT_BLKP(this_block);

			uint32_t      next_blk_size = GET_SIZE(HDRP(next));

			list_index                  = list_hash(next_blk_size); 
			
			list_del(next, list_index);
			
			size += GET_SIZE(HDRP(NEXT_BLKP(this_block)));
			
			PUT(HDRP(this_block), PACK(size, 0));
			PUT(FTRP(this_block), PACK(size, 0));
			
			return this_block;
		}
		else
		{
			//printk("This is the first page, do not combine\n");

			return this_block;
		}

	}
	else
	{
		printk("Colaesce hit 4\n");

		if(fisrt_alloc != 1)
		{
			list_entry_t* next = (list_entry_t*)NEXT_BLKP(this_block);
			list_entry_t* prev = (list_entry_t*)PREV_BLKP(this_block);
			
			size += GET_SIZE(HDRP(NEXT_BLKP(this_block)));
			size += GET_SIZE(HDRP(PREV_BLKP(this_block)));

			uint32_t      next_blk_size = GET_SIZE(HDRP(next));
			uint32_t      prev_blk_size = GET_SIZE(HDRP(prev));

			list_index  = list_hash(prev_blk_size);
			
			list_del(prev, list_index);

			list_index = list_hash(next_blk_size);
			list_del(next, list_index);
			
			PUT(HDRP(PREV_BLKP(this_block)), PACK(size, 0));
			PUT(HDRP(NEXT_BLKP(this_block)), PACK(size, 0));
		
			return (PREV_BLKP(this_block));
		}
		else
		{
			//printk("This is first page, just combine previous\n");

			list_entry_t* prev = (list_entry_t*)PREV_BLKP(this_block);
			
			//size += GET_SIZE(HDRP(NEXT_BLKP(this_block)));
			size += GET_SIZE(HDRP(PREV_BLKP(this_block)));

			//uint32_t      next_blk_size = GET_SIZE(HDRP(next));
			uint32_t      prev_blk_size = GET_SIZE(HDRP(prev));

			list_index  = list_hash(prev_blk_size);
			
			list_del(prev, list_index);

			//list_index = list_hash(next_blk_size);
			//list_del(next, list_index);
			
			PUT(HDRP(PREV_BLKP(this_block)), PACK(size, 0));
			//PUT(HDRP(NEXT_BLKP(this_block)), PACK(size, 0));
		
			return (PREV_BLKP(this_block));
		}
	}
}

void* extend_heap()
{
	uint32_t* block_ptr = NULL;
	uint32_t  size      = PAGE_SIZE;

	//printk("extend heap called\n");

	if(heap_stack == num_of_page)
	{
		//printk("Return Null Pointer\n");
		
		return NULL;
	}
	else
	{
		//printk("Return a Page\n");

		block_ptr = (uint32_t*)stack_heap[heap_stack];

		block_ptr = (uint32_t*)((uint8_t*)block_ptr + DWSIZE);

		struct block_info *this_block = (struct block_info*)HDRP(block_ptr);

		PUT(HDRP(block_ptr), PACK(size, 0));
		PUT(FTRP(HDRP(block_ptr)), PACK(size, 0));

		this_block->allocated = 1;
		this_block->first     = 1;

		((list_entry_t*)block_ptr)->next = NULL;
		((list_entry_t*)block_ptr)->prev = NULL;
		
		heap_stack++;

		//printk_color(black, green,"Current heap stak: %d\n", heap_stack);

		uint8_t list_index = list_hash(PAGE_SIZE);
		
		list_add((list_entry_t*)block_ptr, list_index);

		//printk("%d %d\n", ((struct block_info*)HDRP(block_ptr))->first, ((struct block_info*)HDRP(block_ptr))->allocated);
		//printk("address is 0x%x %d\n", HDRP(block_ptr), GET_SIZE(HDRP(block_ptr)));
		//printk("address is 0x%x %d\n", FTRP(HDRP(block_ptr)), GET_SIZE(FTRP(HDRP(block_ptr))));

		return (void*)block_ptr;
	}
}

bool mm_init()
{
	heap_stack = 0;
	
	//Initialize free list
	for(uint8_t i = 0; i < NUM_OF_LIST; i++)
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
	//printk("find a fit is called\n");

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
	for(list_index = free_list_index; list_index < NUM_OF_LIST; list_index++)
	{

		if(free_list[list_index] != NULL)
		{
			//printk("First Hit list index: %d\n", list_index);
			//Get List Head
			list_elem_ptr = free_list[list_index];
			
			//Get Block Information
			block_size = GET_SIZE(HDRP(list_elem_ptr));
			
			uint32_t left_size = 0;
			//Situation 1

			//printk("block size: %d\n", block_size);
			//printk("alloc size: %d\n", alloc_size);
			
			if(block_size >= alloc_size && (block_size - alloc_size) > MIN_SIZE)
			{
				printk("situation 1\n");
				
				struct block_info* this_block = (struct block_info*)HDRP(list_elem_ptr);
				//printk("first: %d\n", this_block->first);
				//printk("allocate: %d\n", this_block->allocated);

				list_del(list_elem_ptr, list_index);

				//printk("%d %d\n", ((struct block_info*)HDRP(list_elem_ptr))->first, ((struct block_info*)HDRP(list_elem_ptr))->allocated);
				
				remaining_size = block_size - alloc_size;
				
				alloc_ptr = (uint8_t*)HDRP(list_elem_ptr) + remaining_size;		
				alloc_ptr = (void*)((uint8_t*)alloc_ptr + DWSIZE);

				//Mark allocated chunck information
				PUT(HDRP(alloc_ptr), PACK(alloc_size, 1));
				PUT(FTRP(HDRP(alloc_ptr)), PACK(alloc_size, 1));

				if(((struct block_info*)HDRP(list_elem_ptr))->first     == 1 &&
					((struct block_info*)HDRP(list_elem_ptr))->allocated == 1)
				{
					//printk("Update first page allocation\n");

					this_block->allocated = 0; //new page from heap stack
					this_block->first     = 0;
					
					this_block = (struct block_info*)HDRP(alloc_ptr);

					this_block->first = 1;
				}
				
				PUT(HDRP(list_elem_ptr), PACK(remaining_size, 0));
				PUT(FTRP(HDRP(list_elem_ptr)), PACK(remaining_size, 0));

				//Mark leftover chunk information
			
				//Update the free list if remaining size is not zero
				//remaining size < 0 is impossible in this case
				//current design does not allow > 4096 malloc
				if(remaining_size >= 0)
				{
					//printk("List update\n");
					//printk("Remaing size: %d\n", remaining_size);
					list_index    = list_hash(remaining_size);
					//printk("New List Index: %d\n", list_index);
					list_head_ptr = free_list[list_index];

					//if no posiition is NULL
					if(list_head_ptr != NULL)
					{
						//list_add_before(list_elem_ptr);
						list_add(list_elem_ptr, list_index);
					}
					else if(list_head_ptr == NULL)
					{
						//free_list[list_index] = list_elem_ptr;
						list_add(list_elem_ptr, list_index);
					}
					else
					{
						list_add(list_elem_ptr, list_index);
					}
			
					//printk("allocate block size: %d Head Addr: 0x%x\n", GET_SIZE(HDRP(alloc_ptr)), HDRP(alloc_ptr));
					//printk("allocate block size: %d Foot Addr: 0x%x\n", GET_SIZE(FTRP(HDRP(alloc_ptr))), FTRP(HDRP(alloc_ptr)));
					//printk("remaining block size: %d Head Addr: 0x%x\n", GET_SIZE(HDRP(list_elem_ptr)), HDRP(list_elem_ptr));
					//printk("remaining block size: %d Foot Addr: 0x%x\n", GET_SIZE(FTRP(HDRP(list_elem_ptr))), FTRP(HDRP(list_elem_ptr)));

					//printk("This first: %d allocated: %d\n", ((struct block_info*)HDRP(alloc_ptr))->first, ((struct block_info*)HDRP(alloc_ptr))->allocated);

					return (void*)alloc_ptr;
				}
			}
			else if(block_size >= alloc_size && (block_size - alloc_size) <= MIN_SIZE)//Reamaining size is not enough to make a min_size
			{
				printk("Situation 2\n");

				list_del(list_elem_ptr, list_index);
				
				return (void*)list_elem_ptr;
			}
		
			printk("Situation 3\n");
			//if first fit does not work, then traverse
			list_elem_ptr = list_elem_ptr->next;
		
			for(; list_elem_ptr != NULL; list_elem_ptr = list_elem_ptr->next)
			{
				block_size = GET_SIZE(HDRP(list_elem_ptr));

				//printk("Situation 3 in\n");
				
				if(block_size >= alloc_size && (block_size - alloc_size) > MIN_SIZE)
				{
					//printk("Situation 3 hit 1\n");
					list_del(list_elem_ptr, list_index);
					
					left_size = block_size - alloc_size;
					
					alloc_ptr = (uint8_t*)HDRP(list_elem_ptr) + remaining_size;
					alloc_ptr = (void*)((uint8_t*)alloc_ptr + DWSIZE);
					
					//Update Footer and Header
					PUT(HDRP(alloc_ptr), PACK(alloc_size, 1));
					PUT(FTRP(alloc_ptr), PACK(alloc_size, 1));
					
					PUT(HDRP(list_elem_ptr),  PACK(left_size, 0));
					PUT(FTRP(list_elem_ptr),  PACK(left_size, 0));
					
					if(left_size >= 0)
					{
						list_index      = list_hash(left_size);
						list_head_ptr   = free_list[list_index];
						
						if(list_head_ptr != NULL)
						{
							list_add_before(list_head_ptr, list_elem_ptr);
						}
						else if(list_head_ptr == NULL)
						{
							//free_list[list_index] = list_elem_ptr;
							list_add(list_elem_ptr, list_index);
						}
						else
						{
							list_add(list_elem_ptr, list_index);
						}
						
						//printk("block size: %d\n", GET_SIZE(HDRP(alloc_ptr)));
						//printk("block size: %d\n", GET_SIZE(FTRP(alloc_ptr)));

						return (void*)alloc_ptr;
					}
				}
				else if(block_size >= alloc_size && (block_size - alloc_size) <= MIN_SIZE)
				{
					//printk("Situation 3 hit 2\n");

					list_del(list_elem_ptr, list_index);
					
					return (void*)list_elem_ptr;
				}
			}

			//printk("There is no fit\n");
		}
	}

	//printk("There is no list for search\n");

	return NULL;
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
	
	uint8_t list_index = list_hash(size);
	
	list_add((list_entry_t*)coalesce(this_chunk), list_index);

	this_chunk = NULL;
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
		//mm_mark_used(alloc_ptr, alloc_size - OVERHEAD);
		
		return alloc_ptr;
	}
	
	extend_size         = alloc_size;

	uint8_t* new_chunck = NULL;
	
	if((new_chunck = extend_heap()) != NULL)
	{
		if((alloc_ptr = mm_search_fit(alloc_size)) != NULL)
		{
			//mm_mark_used(alloc_ptr, alloc_size - OVERHEAD);
			
			return alloc_ptr;
		}
		else
		{
			return NULL;
		}

	}
	
	return alloc_ptr;
		
}

void free_list_dump()
{
	for(uint32_t i = 0; i < NUM_OF_LIST; i++)
	{
		if(free_list[i] != NULL)
		{
			for(list_entry_t* curr = free_list[i]; curr != NULL; curr = curr->next)
			{
				printk("Chunk Address is: 0x%x", curr);
			}
		}
	}
}
