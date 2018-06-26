#ifndef _GDT_H_
#define _GDT_H_

typedef struct
{
	uint16_t	limit_low;
	uint16_t    base_low;
	uint8_t		base_mid;
	union
	{
		struct
		{
			uint8_t		attr;
			uint8_t     limit;
		};
		uint16_t limit_attr;
	};
	uint8_t     base_high;
}__attribute__((packed))gdt_descriptor_t;

typedef struct
{
	uint16_t 	offset_low;
	uint16_t 	selector;
	union
	{
		struct
		{
			uint8_t dcount;
			uint8_t attr;
		};
		uint16_t attribute;
	};
	uint16_t    offset_high;
}__attribute__((packed))gdt_gate_t;


#define index_gdt_default		0x0
#define index_gdt_sys_code		0x1
#define index_gdt_sys_data		0x2
#define index_gdt_video			0x3 //depend on change
//#define index_gdt_usr_code	0x3
//#define index_gdt_usr_data    0x4
//#define index_tss             0x5

#define selector_gdt_default	0x0
#define selector_gdt_sys_code   0x1
#define selector_gdt_sys_data   0x2
#define selector_gdt_usr_code   0x3
#define selector_gdt_usr_data   0x4
#define selector_tss            0x5 

#define	DA_32					0x4000	/* 32 bits segment			*/
#define	DA_LIMIT_4K				0x8000	/* 			*/
#define	DA_DPL0					0x00	/* DPL = 0				*/
#define	DA_DPL1					0x20	/* DPL = 1				*/
#define	DA_DPL2					0x40	/* DPL = 2				*/
#define	DA_DPL3					0x60	/* DPL = 3				*/

#define	DA_DR					0x90	
#define	DA_DRW					0x92	
#define	DA_DRWA					0x93	
#define	DA_C					0x98	
#define	DA_CR					0x9A	
#define	DA_CCO					0x9C	
#define	DA_CCOR					0x9E	

#define	DA_LDT					0x82	
#define	DA_TaskGate				0x85	
#define	DA_386TSS				0x89
#define	DA_386CGate				0x8C	
#define	DA_386IGate				0x8E	
#define	DA_386TGate				0x8F	


#define	INT_VECTOR_DIVIDE		0x0
#define	INT_VECTOR_DEBUG		0x1
#define	INT_VECTOR_NMI			0x2
#define	INT_VECTOR_BREAKPOINT	0x3
#define	INT_VECTOR_OVERFLOW		0x4
#define	INT_VECTOR_BOUNDS		0x5
#define	INT_VECTOR_INVAL_OP		0x6
#define	INT_VECTOR_COPROC_NOT	0x7
#define	INT_VECTOR_DOUBLE_FAULT	0x8
#define	INT_VECTOR_COPROC_SEG	0x9
#define	INT_VECTOR_INVAL_TSS	0xA
#define	INT_VECTOR_SEG_NOT		0xB
#define	INT_VECTOR_STACK_FAULT	0xC
#define	INT_VECTOR_PROTECTION	0xD
#define	INT_VECTOR_PAGE_FAULT	0xE
#define	INT_VECTOR_COPROC_ERR	0x10


#define	INT_VECTOR_IRQ0			0x20
#define	INT_VECTOR_IRQ8			0x28

#define gdt_descriptor_set(a,x,y,z)({	\
	((a)).limit_low       = ((y) & 0xFFFF);	\
	((a)).base_low  	  = ((x) & 0xFFFF);	\
	((a)).base_mid  	  = (((x) >> 16) & 0xFF);	\
	((a)).limit_attr      = ((((y) >> 8) & 0x0F00) | (((z) & 0xF0FF)));	\
	((a)).base_high       = (((x) >> 24) & 0xFF);	})

#define gdt_gate_set(a,x,y,z,w)({	\
	((a)).offset_low      = ((y) & 0xFFFF);	\
	((a)).selector        = (x);	\
	((a)).attribute       = (((z) & 0x1F) | (((w) << 8) & 0xFF00));	\
	((a)).offset_high     = (((y) >> 16) & 0xFFFF);	\})
  
#endif
