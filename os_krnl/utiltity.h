#ifdef _UTILITY_H_
#define _UTILITY_H_

typedef struct list_element
{
	struct list_element* p_prev;
	struct list_element* p_next;
}list_entry_t;

bool list_add(void* element, void* list_head);
bool list_remove(void* element, void* list_head);
void* list_search(void* element);

#endif
