
#include "types.h"
#include "debug.h"
#include "pmm.h"

#define NUM_MEM_SEG_MAX   32

mem_seg_entry_t[NUM_MEM_SEG_MAX];

bool init_mem_seg_entry()
{
  uint32_t i = 0;
  
  for(; i < NUM_MEM_SEG_MAX; i++)
  {
    mem_seg_entry_t[i].size          = 0xFFFFFFFF;
    mem_seg_entry_t[i].base_addr_low = 0xFFFFFFFF;
    mem_seg_entry_t[i].base_addr_hi  = 0xFFFFFFFF;
    mem_seg_entry_t[i].length_low    = 0xFFFFFFFF;
    mem_seg_entry_t[i].length_hi     = 0xFFFFFFFF;
    mem_seg_entry_t[i].type          = 0xFFFFFFFF;
  }
  
  return true;
}

bool copy_mem_seg_entry()
{
  uint32_t effective_segs = 0;
  uint32_t seg_index      = 0;
  
  if(get_num_mem_s(&effective_segs) != 0)
  {
    MemCopy(mem_seg_entry_t, effective_segs*sizeof(mem_seg_entry_t));
  }
  else
  {
    debug_print();
  }
}
