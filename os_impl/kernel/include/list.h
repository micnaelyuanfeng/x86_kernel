#ifndef LIST_H_
#define LIST_H_


/*************************************************************************
 * Basic Constants and Macros
 * You are not required to use these macros but may find them helpful.
 *************************************************************************/
struct list_entry 
{
    struct list_entry *prev;
    struct list_entry *next;
};

typedef struct list_entry list_entry_t; 

#define NUM_OF_LIST                 16

//list management functions
bool      list_add(list_entry_t *this_element, uint8_t list_index);
bool      list_add_before(list_entry_t *this_element, list_entry_t *elm) ;
bool      list_del(list_entry_t *this_element, uint8_t list_index); 
bool      list_init(list_entry_t* this_element);
uint32_t  next_pow_of_2(uint32_t size);
uint32_t  is_pow_of_2(uint32_t size);
uint8_t   list_hash(uint32_t size);

#endif