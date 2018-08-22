#include "types.h"
#include "color.h"
#include "display.h"
#include "lib.h"
#include "idt.h"
#include "timer.h"

uint32_t tick;

void timer_handler(proc_ctx_t* this_context)
{
    //printk_color(black, white, "Tick: %d\r", tick++);
}

void init_timer(uint32_t freq)
{
    tick = 0;

    register_interrupt_handler(IRQ0, timer_handler);

    uint32_t divisor = 1193180 / freq;

    outb(0x43, 0x36);

    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = ((uint8_t) ((divisor >> 8) & 0xFF));

    outb(0x40, low);
    outb(0x40, high);
}
