//#include "types.h"
#include "gdt.h"


seg_desc_t gdt[3] = {
	[0] = SEG_DESC(0,0,0,0,0,0,0,0),
	[1] = SEG_DESC(0x7c80, 0xFF, TYPE_CODE_XO, DESC_TYPE_CODE, DPL_0, SEG_PRESENT, CODE_32, G_BYTE),
	[2] = SEG_DESC(0xb8000, 0xFFFF, TYPE_DATA_RW, DESC_TYPE_DATA, DPL_0, SEG_PRESENT, DATA_32, G_BYTE),
};

gdtr_t gdtr = {
	.limit = sizeof(gdt) - 1,
	.base  = (uint32_t)&gdt,
};