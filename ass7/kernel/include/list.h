#ifndef _LIST_H_
#define _LIST_H_

#define WORD_SIZE				sizeof(void*)
#define DWORD_SIZE				(WORD_SIZE << 1)
#define OVERHEAD				DWORD_SIZE

#define MAX(x,y)

struct list_entry_desc;

typedef struct list_entry_desc
{
	struct
	{
		uint32_t allocated	:	1;
		uint32_t size       :	29;
		uint32_t reserved   :   1;
	}info;

	list_entry_t* p_prev;
	list_entry_t* p_next;
}list_entry_t;

inline bool     list_add(void* this_element)__attribute__((always_inline));
inline bool     list_remove(void* this_element)__attribute__((always_inline));
inline bool     list_search(void* this_element)__attribute__((always_inline));
inline uint32_t list_hash(uint32_t size);

inline void     is_pow_of_2(uint32_t size)__attribute__((always_inline));
inline uint32_t next_pow_of_2(uint32_t size)__attribute__((always_inline));
inline void     find_fit(uint32_t size);
#endif