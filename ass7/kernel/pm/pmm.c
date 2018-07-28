#include "types.h"
#include "display.h"
#include "pmm.h"

void* heap_list = NULL;

/*
	Aussume largest allocated chunk size = 4KB
	Any chunk > 4KB will be put in the last
	list of the array
*/
list_entry_t* free_list[12];

void mm_init(void)
{
	
}

void* mm_malloc(uint32_t size)
{

}

void* mm_realloc(void* this_mem_ptr, uint32_t size)
{
	
}

void* mm_free(void* this_mem_ptr)
{
	
}

void* mm_coalesce(void* this_mem_ptr)
{
	
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
	
	list_entry_t* list_ptr      = NULL;
	list_entry_t* traverse_ptr  = NULL;
	list_entry_t* list_elem_ptr = NULL;
	list_entry_t* remaining_ptr = NULL:
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
			}
			
			//Update the free list
			if(remaining_size > 0)
			{
			}
		}
	}
}

void  mm_mark_used(void* this_mem_ptr, uint32_t size)
{
	
}	
