#ifndef _VGA_H_
#define _VGA_H_

//Define Color Scheme
//Later define more color = research on Open VGA

//#definition R G B
#define COLOR_000000                0
#define COLOR_FF0000                1
#define COLOR_00FF00                2
#define COLOR_FFFF00                3
#define COLOR_0000FF                4
#define COLOR_FF00FF                5
#define COLOR_00FFFF                6
#define COLOR_FFFFFF                7
#define COLOR_C6C6C6                8
#define COLOR_840000                9
#define COLOR_008400                10
#define COLOR_848400                11
#define COLOR_000084                12
#define COLOR_840084                13
#define COLOR_008484                14
#define COLOR_848484                15

#define PALETTE_NUM_REG_OFFSET      0x03C8
#define PALETTE_RGB_REG_OFFSET      0x03C9

void init_palette(void);

void set_palette(uint32_t start, uint32_t end, uint8_t* rgb);

void draw_main(void);

void draw(uint8_t* vga_addr, uint32_t x_range, uint8_t color_rgb, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);

#endif