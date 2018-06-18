#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#define ZEROPAD		1		// pad with zero
#define SIGN	 	2   	// unsigned/signed long
#define PLUS    	4		// show plus
#define SPACE	  	8   	// space if plus
#define LEFT	 	16  	// left justified
#define SPECIAL		32  	// 0x
#define SMALL		64 		// use 'abcdef' instead of 'ABCDEF'

typedef __builtin_va_list va_list;

#define va_start(ap, last)         (__builtin_va_start(ap, last))
#define va_arg(ap, type)           (__builtin_va_arg(ap, type))
#define va_end(ap)

void move_cursor(void);
void console_clear(void);
void scroll(void);

void console_test(void);
void console_write(char *cstr);
void console_putc_color(char c, color_type_t back, color_type_t front);
void console_write_color(char *cstr, color_type_t back, color_type_t front);

int vsprintf(char *buff, const char *format, va_list args);
int skip_atoi(const char **s);
char* itoa(char* str, uint32_t num, uint32_t base);
char* number(char *str, int num, int base, int size, int precision, int type);

void printk(const char *format, ...);
void printk_int(uint32_t num);
void printk_color(color_type_t back, color_type_t fore, const char *format, ...);
void printk_int_colour(uint32_t num, color_type_t back, color_type_t fore);

#define do_div(n,base) ({ \
		int __res; \
		__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
		__res; })



#endif