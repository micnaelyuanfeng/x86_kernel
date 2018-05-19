#ifndef _GDT_H_
#define _GDT_H_
//#include "stdint.h"
// Type field
#define TYPE_DATA_RO            0
#define TYPE_DATA_RO_A          1
#define TYPE_DATA_RW            2
#define TYPE_DATA_RW_A          3
#define TYPE_DATA_RO_E          4
#define TYPE_DATA_RO_E_A        5
#define TYPE_DATA_RW_E          6
#define TYPE_DATA_RW_E_A        7
#define TYPE_CODE_XO            8
#define TYPE_CODE_XO_A          9
#define TYPE_CODE_XR            10
#define TYPE_CODE_XR_A          11
#define TYPE_CODE_XO_C          12
#define TYPE_CODE_XO_C_A        13
#define TYPE_CODE_XR_C          14
#define TYPE_CODE_XR_C_A        15

#define TYPE_SYS_TSS_16_A       1
#define TYPE_SYS_LDT            2
#define TYPE_SYS_TSS_16_B       3
#define TYPE_SYS_CALL_GATE_16   4
#define TYPE_SYS_TASK_GATE      5
#define TYPE_SYS_INT_GATE_16    6
#define TYPE_SYS_TRAP_GATE_16   7
#define TYPE_SYS_TSS_32_A       9
#define TYPE_SYS_TSS_32_B       11
#define TYPE_SYS_CALL_GATE_32   12
#define TYPE_SYS_INT_GATE_32    14
#define TYPE_SYS_TRAP_GATE_32   15

// S(descripor type) flag
#define DESC_TYPE_SYSTEM 		0
#define DESC_TYPE_CODE   		1
#define DESC_TYPE_DATA   		1

// DPL(descriptor privilege level) field
#define DPL_0 					0
#define DPL_1 					1
#define DPL_2 					2
#define DPL_3 					3

// P(segment-present) flag
#define SEG_PRESENT     		1
#define SEG_NOT_PRESENT 		0

// D/B flag
#define CODE_32         		1
#define CODE_16         		0
#define DATA_32         		1
#define DATA_16         		0

// G(granularity) flag
#define G_BYTE    				0
#define G_4K_BYTE 				1

typedef struct __attribute__((packed)) 
{
    uint16_t limit;
    uint32_t base;
} gdtr_t;

typedef struct 
{ 
    uint64_t limit_15_0 : 16;
    uint64_t base_23_0  : 24;
    uint64_t type       : 4;
    uint64_t s          : 1;
    uint64_t dpl        : 2;
    uint64_t p          : 1;
    uint64_t limit_19_16: 4;
    uint64_t avl        : 1;
    uint64_t res        : 1;
    uint64_t db         : 1;
    uint64_t g          : 1;
    uint64_t base_31_24 : 8;
} seg_desc_t;

#define SEG_DESC(_base, _limit, _type, _s, _dpl, _p, _db, _g) { \
        .base_23_0 = (_base) & 0xffffff,         \
        .base_31_24 = ((_base) >> 24) & 0xff, \
        .limit_15_0 = (_limit) & 0xffff, \
            .limit_19_16 = ((_limit) >> 16) & 0xf, \
            .type = (_type), \
            .s = (_s), \
            .dpl = (_dpl), \
            .p = (_p), \
            .avl = 0, \
            .res = 0, \
            .db = (_db), \
            .g = (_g), \
}

#endif