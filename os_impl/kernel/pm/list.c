#include "types.h"
#include "color.h"
#include "display.h"
#include "list.h"
#include "pmm.h"

extern list_entry_t* free_list[NUM_OF_LIST]; //Question about *free_list only = compile error

bool list_add(list_entry_t* this_element, uint8_t list_index)
{
	bool result = true;
	
	if(this_element == NULL)
	{
		result = false;
	}
	
	if(result)
	{
		printk("add to list index: %d\n", list_index);

		if(free_list[list_index] != NULL)
		{
			printk("Add to a list\n");
			result = list_add_before(free_list[list_index], this_element);
		}
		else
		{
			printk("Make a list\n");
			free_list[list_index] = this_element;
			
			free_list[list_index]->prev = NULL;
			free_list[list_index]->next = NULL;
		}
	}
	
	return result;
}

bool list_add_before(list_entry_t* list_head, list_entry_t* this_element)
{
	bool result = true;
	
	list_entry_t* temp = NULL;
	
	temp = list_head;
	
	list_head = this_element;
	
	this_element->next = temp;
	this_element->prev = NULL;
	
	temp->prev          = this_element;

	return result;
}

bool list_del(list_entry_t* this_element, uint8_t list_index)
{
	bool result = true;
	
	if(this_element == NULL)
	{
		result = false;
	}
	
	if(result)
	{
		if(free_list[list_index] != this_element)
		{
			list_entry_t* prev = NULL;
			list_entry_t* curr = free_list[list_index];
			
			for(; curr != NULL; curr = curr->next)
			{
				if(curr == this_element)
				{
					//middle
					//tail
					if(curr->next == NULL)
					{
						prev->next = NULL;
						curr->next = NULL;
						curr->prev = NULL;
					}
					else
					{
						prev->next       = curr->next;
						curr->next->prev = prev;
						
						curr->next = NULL;
						curr->prev = NULL;
					}
				}
				
				prev = curr;
			}
		}
		else
		{
			if(free_list[list_index]->next != NULL)
			{
				this_element->prev->next = NULL;
				
				this_element->next = NULL;
				this_element->prev = NULL;
			}
			else
			{
				//only one element
				free_list[list_index] = NULL;
			
				this_element->next = NULL;
				this_element->prev = NULL;
			}
	
		}
	
	}
	
	return result;
}

uint8_t list_hash(uint32_t size)
{
   
   uint8_t counter = 0;

   //assert(size != 0);

   while(size)
   {
		counter ++;
		size = size >> 1;
   }

   if (counter >= 15)
   {
	    return 15;
   }

	return counter;	    
}

uint32_t is_pow_of_2(uint32_t size)
{
	return !(size & (size - 1));
}

uint32_t next_pow_of_2(uint32_t size)
{
    if(is_pow_of_2(size))
	{
        return size;
    }
	else
	{
        size |= size >> 1;
        size |= size >> 2;
        size |= size >> 4;
        size |= size >> 8;
        size |= size >> 16;

        size++;
		
        return size;
    }
	
	return size;
}
