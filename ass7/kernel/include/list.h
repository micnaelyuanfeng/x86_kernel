#ifndef LIST_H_
#define LIST_H_


/*************************************************************************
 * Basic Constants and Macros
 * You are not required to use these macros but may find them helpful.
 *************************************************************************/
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

//list management functions
inline void list_add(list_entry_t *elm) __attribute__((always_inline));
inline void list_add_before(list_entry_t *listelm, list_entry_t *elm) __attribute__((always_inline));
inline void list_del(list_entry_t *listelm) __attribute__((always_inline));
inline uint32_t next_pow_of_2(uint32_t size) __attribute__((always_inline));
inline uint32_t is_pow_of_2(uint32_t size) __attribute__((always_inline));
inline uint8_t list_hash(uint32_t size) __attribute__((always_inline));


#endif