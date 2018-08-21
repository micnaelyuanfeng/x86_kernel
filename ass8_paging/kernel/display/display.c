#include "types.h"
#include "color.h"
#include "display.h"
#include "lib.h"
#include "vm.h"

//GS segment register will be wiped out, so video memory use
//hard code address instead before paging is enable
uint16_t *video_memory = (uint16_t *)0xB8000;

extern uint8_t cursor_x;
extern uint8_t cursor_y;
extern uint16_t disp_pos;

void disp_str(char * pszInfo);
void disp_color_str(char * info, int color);

void set_cursor()
{
	uint16_t location = 0 * 80 + 0;

	outb(0x3d4, 0xF);
	outb(0x3d5, (uint8_t)(location & 0xFF));

	outb(0x3d4, 0xFE);
	outb(0x3d5, (uint8_t)((location >> 8) & 0xFF));
}

void move_cursor(void)
{
  uint16_t cursorLocation = cursor_y * 80 + cursor_y;

  outb(0x3d4, 14);
  outb(0x3d5, cursorLocation >> 8);
  outb(0x3d4, 15);
  outb(0x3d5, cursorLocation);
}

void console_test(void)
{
	;
}

static void swap(char* a, char* b)
{
	char middle;

	middle = *a;

	*a = *b;

	*b = middle;
}

static void reverse(char* str, int length)
{
	uint32_t start = 0;
	uint32_t end   = length - 1;

	while(start < end)
	{
		swap((str + start), (str + end));

		start ++;
		end --;
	}
}

void console_clear(void)
{
  uint8_t attribute_byte = (0 << 4) | (15 & 0x0F);

  uint16_t blank = 0x20 | (attribute_byte << 8);

  uint16_t i;
  
  for(i = 0; i < 80 * 28; i ++)
  {
    video_memory[i] = blank;
  }

  cursor_x = 0;
  cursor_y = 0;
  
  move_cursor();
}

void console_write(char *cstr)
{
  while(*cstr)
  {
    console_putc_color(*cstr++, black, white);
  }
}

void console_write_color(char *cstr, color_type_t back, color_type_t front)
{
  while(*cstr)
  {
    console_putc_color(*cstr++, back, front);
  }
}

void scoll()
{
  uint8_t attribute_byte = (0 << 4)|(15 & 0x0f);
  uint16_t blank = 0x20 | (attribute_byte << 8);

  if(cursor_y >= 25)
  {
    uint16_t i;

    for(i= 0; i < 24 * 80; i++)
    {
      video_memory[i] = video_memory[i+80];
    }

    for (i = 24 * 80; i < 25 * 80; i++)
    {
      video_memory[i] = blank;
    }

    cursor_y = 24;
  }
}

void console_putc_color(char c, color_type_t back, color_type_t front)
{
  //uint8_t back_color = (uint8_t) back;
  //uint8_t front_color = (uint8_t) front;

  uint8_t attribute_byte = (back << 4) | (front & 0x0f);
  uint16_t attribute = attribute_byte << 8;

  //tab = 0x09 and space = 0x08
  if (c == 0x08 && cursor_x)
  {
      cursor_x--;
  }
  else if(c == 0x09)
  {
    cursor_x = (cursor_x + 8) & ~(8 - 1);
  }
  else if(c == '\r')
  {
    cursor_x = 0;
  }
  else if(c == '\n')
  {
    cursor_x = 0;
    cursor_y++;
  }
  else if(c >= ' ')
  {
    video_memory[cursor_y * 80 + cursor_x] = c | attribute;
    cursor_x++;
  }

  if(cursor_x >= 80)
  {
    cursor_x = 0;
    cursor_y ++;
  }

  scoll();

  move_cursor();
}

//---------------------print function-------------------------------//
int vsprintf(char *buff, const char *format, va_list args);

int skip_atoi(const char **s)
{
	int i = 0;

	while (is_digit(**s)) {
		i = i * 10 + *((*s)++) - '0';
	}

	return i;
}

char* itoa(char* str, uint32_t num, uint32_t base)
{
	uint32_t i = 0;

	bool isNegative = false;

	if(num == 0)
	{
		str[i++] = '0';
        str[i] = '\0';
        return str;
	}

	if (num < 0 && base == 10)
    {
        isNegative = true;

        num = -num;
    }

    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';

    reverse(str, i);

    return str;
}


void printk(const char *format, ...)
{
	static char buff[1024];
	
	va_list args;
	
	int i;

	va_start(args, format);

	i = vsprintf(buff, format, args);

	va_end(args);

	buff[i] = '\0';

	console_write(buff);
}

void printk_color(color_type_t back, color_type_t fore, const char *format, ...)
{
	static char buff[1024];

	va_list args;

	int i;

	va_start(args, format);
	i = vsprintf(buff, format, args);
	va_end(args);

	buff[i] = '\0';

	console_write_color(buff, back, fore);
}

char *number(char *str, int num, int base, int size, int precision, int type)
{
	char c, sign, tmp[36];
	const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	if (type & SMALL) {
		digits ="0123456789abcdefghijklmnopqrstuvwxyz";
	}
	if (type & LEFT) {
		type &= ~ZEROPAD;
	}
	if (base < 2 || base > 36) {
		return 0;
	}

	c = (type & ZEROPAD) ? '0' : ' ' ;

	if (type & SIGN && num < 0) {
		sign = '-';
		num = -num;
	} else {
		sign = (type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
	}

	if (sign) {
	      size--;
	}

	if (type & SPECIAL) {
		if (base == 16) {
			size -= 2;
		}
		else if (base == 8) {
			size--;
		}
	}

	i = 0;
	if (num == 0) {
		tmp[i++] = '0';
	} else {
		while (num != 0) {
			tmp[i++] = digits[do_div(num,base)];
		}
	}

	if (i > precision) {
		precision = i;
	}
	size -= precision;

	if (!(type&(ZEROPAD+LEFT))) {
		while (size-- > 0) {
			*str++ = ' ';
		}
	}
	if (sign) {
		*str++ = sign;
	}
	if (type & SPECIAL) {
		if (base == 8) {
			*str++ = '0';
		} else if (base == 16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	}
	if (!(type&LEFT)) {
		while (size-- > 0) {
			*str++ = c;
		}
	}
	while (i < precision--) {
		*str++ = '0';
	}
	while (i-- > 0) {
		*str++ = tmp[i];
	}
	while (size-- > 0) {
		*str++ = ' ';
	}

	return str;
}

int vsprintf(char *buff, const char *format, va_list args)
{
	int len;
	int i;
	char *str;
	char *s;
	int *ip;

	int flags;		// flags to number()

	int field_width;	// width of output field
	int precision;		// min. # of digits for integers; max number of chars for from string

	for (str = buff ; *format ; ++format) {
		if (*format != '%') {
			*str++ = *format;
			continue;
		}

		flags = 0;
		repeat:
			++format;		// this also skips first '%'
			switch (*format) {
				case '-': flags |= LEFT;
					  goto repeat;
				case '+': flags |= PLUS;
					  goto repeat;
				case ' ': flags |= SPACE;
					  goto repeat;
				case '#': flags |= SPECIAL;
					  goto repeat;
				case '0': flags |= ZEROPAD;
					  goto repeat;
			}

		// get field width
		field_width = -1;
		if (is_digit(*format)) {
			field_width = skip_atoi(&format);
		} else if (*format == '*') {
			// it's the next argument
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		// get the precision
		precision = -1;
		if (*format == '.') {
			++format;
			if (is_digit(*format)) {
				precision = skip_atoi(&format);
			} else if (*format == '*') {
				// it's the next argument
				precision = va_arg(args, int);
			}
			if (precision < 0) {
				precision = 0;
			}
		}

		// get the conversion qualifier
		//int qualifier = -1;	// 'h', 'l', or 'L' for integer fields
		if (*format == 'h' || *format == 'l' || *format == 'L') {
			//qualifier = *format;
			++format;
		}

		switch (*format) {
		case 'c':
			if (!(flags & LEFT)) {
				while (--field_width > 0) {
					*str++ = ' ';
				}
			}
			*str++ = (unsigned char) va_arg(args, int);
			while (--field_width > 0) {
				*str++ = ' ';
			}
			break;

		case 's':
			s = va_arg(args, char *);
			len = strlen(s);
			if (precision < 0) {
				precision = len;
			} else if (len > precision) {
				len = precision;
			}

			if (!(flags & LEFT)) {
				while (len < field_width--) {
					*str++ = ' ';
				}
			}
			for (i = 0; i < len; ++i) {
				*str++ = *s++;
			}
			while (len < field_width--) {
				*str++ = ' ';
			}
			break;

		case 'o':
			str = number(str, va_arg(args, unsigned long), 8,
				field_width, precision, flags);
			break;

		case 'p':
			if (field_width == -1) {
				field_width = 8;
				flags |= ZEROPAD;
			}
			str = number(str, (unsigned long) va_arg(args, void *), 16,
				field_width, precision, flags);
			break;

		case 'x':
			flags |= SMALL;
		case 'X':
			str = number(str, va_arg(args, unsigned long), 16,
				field_width, precision, flags);
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			str = number(str, va_arg(args, unsigned long), 10,
				field_width, precision, flags);
			break;
		case 'b':
			str = number(str, va_arg(args, unsigned long), 2,
				field_width, precision, flags);
			break;

		case 'n':
			ip = va_arg(args, int *);
			*ip = (str - buff);
			break;

		default:
			if (*format != '%')
				*str++ = '%';
			if (*format) {
				*str++ = *format;
			} else {
				--format;
			}
			break;
		}
	}
	*str = '\0';

	return (str -buff);
}

void printk_int(uint32_t num)
{
	char output[16];

	itoa(output, num, 10);
	
	printk(output);
}

void printk_hex(uint32_t num)
{
	char output[16];

	itoa(output, num, 16);

	printk(output);
}

void printk_int_colour(uint32_t num, color_type_t back, color_type_t fore)
{
	char output[16];

	itoa(output, num, 10);
	
	printk_color(back, fore, output);
}
