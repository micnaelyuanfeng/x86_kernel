#include "types.h"

typedef struct gdt_descriptor
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_mid;
	uint8_t  attr;
	uint8_t  limit_high_attr;
	uint8_t  base_high;
}gdt_entry_t;

uint8_t gdtPtr[6];

#define GDT_SIZE		128

gdt_entry_t	gdt[GDT_SIZE];

void* MemCopy(void* p_dest, void* p_src, int size);

void cstart()
{
	MemCopy(&gdt, (void*)(*((uint32_t*)(&gdtPtr[2]))), *((uint16_t*)(&gdtPtr[0])) + 1);
}