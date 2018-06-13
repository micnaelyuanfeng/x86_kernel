typedef struct memory_segment
{
  uint32_t size;
  uint32_t base_addr_low;
  uint32_t base_add_high;
  uint32_t length_low;
  uint32_t length_high;
}__attribute__((packed))mem_seg_entry_t;
