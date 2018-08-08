#include "types.h"
#include "display.h"
#include "pmm.h"

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
	
	list_entry_t* list_head_ptr = NULL;
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
			
				//Update the free list if remaining size is not zero
				if(remaining_size > 0)
				{
					list_index    = list_hash(remaining_size);
					list_head_ptr = free_list[list_index];

					//if no posiition is NULL
					if(list_head_ptr != NULL)
					{
						list_add_before();
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
						else if(list_head_ptr == NULL;)
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
	if(this_chunck == NULL)
	{
		return;
	}
	
	uint32_t size = GET_SIZE(HDRP(this_chunck));
	
	PUT(HDRP(this_chunck), PACK(size,0));
    PUT(FTRP(this_chunck), PACK(size,0));
	
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
	
	if(size <= DSIZE)
	{
		alloc_size = DSIZE + OVERHEAD;
	}
	else
	{
		alloc_size = DSIZE * ((size + (OVERHEAD) + (DSIZE-1))/ DSIZE);
	}
	
	if((alloc_ptr = find_fit(alloc_size)) != NULL)
	{
		mm_mark_used(alloc_ptr, alloc_size);
		
		return alloc_ptr;
	}
	
	extend_size = alloc_size;
	
	if((alloc_ptr = extend_heap(extend_size / WSIZE)) == NULL)
	{
		return NULL;
	}
	
	mm_mark_used(alloc_ptr, alloc_size);
	
	return alloc_ptr;
	
	
}
