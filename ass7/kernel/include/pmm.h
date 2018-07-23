#ifndef _PMM_H_
#define _PMM_H_

int mm_init(void);

void* mm_malloc(uint32_t size);
void  mm_free(void* ptr);
void* mm_realloc(void* ptr, uint32_t size);

#endif