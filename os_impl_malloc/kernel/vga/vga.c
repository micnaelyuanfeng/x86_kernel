#include "types.h"
#include "vga.h"
#include "color.h"
#include "display.h"
#include "lib.h"

//uint32_t does not work
uint8_t* video_buffer = (uint8_t *)0xA0000;

static uint8_t rgb_table[48] =
{
//  R     G     B
    0x00, 0x00, 0x00,
    0xff, 0x00, 0x00,
    0x00, 0xff, 0x00,
    0xff, 0xff, 0x00,
    0x00, 0x00, 0xff,
    0xff, 0x00, 0xff,
    0x00, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xc6, 0xc6, 0xc6,
    0x84, 0x00, 0x00,
    0x00, 0x84, 0x00,
    0x84, 0x84, 0x00,
    0x00, 0x00, 0x84,
    0x84, 0x00, 0x84,
    0x00, 0x84, 0x84, 
    0x84, 0x84, 0x84
};

void init_palette(void)
{
    //printk("Init Start\n");

    set_palette(0, 15, rgb_table);

    //printk("Init Done\n");
}

void set_palette(uint32_t start, uint32_t end, uint8_t* rgb)
{
    disable_int();

    //printk("Set Init1\n");

    outb(PALETTE_NUM_REG_OFFSET, start);

    for(uint8_t i = 0; i <= end; i++)
    {
        outb(PALETTE_RGB_REG_OFFSET, *(rgb + 0)/4);
        outb(PALETTE_RGB_REG_OFFSET, *(rgb + 1)/4);
        outb(PALETTE_RGB_REG_OFFSET, *(rgb + 2)/4);

        rgb += 3;
    }

    //printk("Set Init3\n");
    
    //enable_int();
}

void draw_main(void)
{
    uint32_t x_range, y_range;

    init_palette();

    x_range = 640;
    y_range = 350;

    draw(video_buffer, x_range, COLOR_848484, 0,  0,           x_range - 1, y_range - 20);
    draw(video_buffer, x_range, COLOR_C6C6C6, 0,  y_range - 20, x_range - 1, y_range - 20);
    //draw(video_buffer, x_range, COLOR_FF00FF, 0,  y_range - 20, x_range - 1, y_range - 20);
    //draw(video_buffer, x_range, COLOR_00FF00, 0,  y_range - 20, x_range - 1, y_range - 20);
    //draw(video_buffer, x_range, COLOR_00FF00, 0,  y_range - 20, x_range - 1, y_range - 20);
    draw(video_buffer, x_range, COLOR_00FF00, 0,  y_range - 20, x_range - 1, y_range - 20);
    //draw();

}

void draw(uint8_t* vga_addr, uint32_t x_range, uint8_t color_rgb, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
{
    uint8_t x,y;

    for(y = y0; y <= y1; y++)
    {
        for(x = x0; x <= x1; x++)
        {
            vga_addr[y * x_range + x] = color_rgb;
        }
    }
}
