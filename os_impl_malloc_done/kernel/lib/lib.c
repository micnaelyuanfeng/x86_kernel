#include "types.h"
#include "lib.h"

void outb(uint16_t port_idx, uint8_t write_value)
{
	__asm__ volatile ("outb %1, %0" : : "dN"(port_idx), "a"(write_value));
}

uint8_t inb(uint16_t port_idx)
{
	uint8_t ret;

	__asm__ volatile ("inb %1, %0" : "=a"(ret) : "dN"(port_idx));

	return ret;
}

uint16_t inw(uint16_t port_idx)
{
	uint16_t ret;

	__asm__ volatile("inw %1, %0" : "=a"(ret) : "dN"(port_idx));

	return ret;
}

void insl(uint32_t port, void * addr, uint32_t n)
{
  __asm__ volatile ("cld");
  
  __asm__ volatile ("repne \n"
                "insl"
                :"=D"(addr), "=c"(n)
                :"d"(port), "0"(addr), "l"(n)
                : "memory", "cc");
}

void outsl(uint32_t port, void * addr, uint32_t n)
{
  __asm__ volatile ("cld");
  
  __asm__ volatile ("repne \n"
                "outsl"
                :"=D"(addr), "=c"(n)
                :"d"(port), "0"(addr), "l"(n)
                : "memory", "cc");
}

void enable_int(void)
{
	__asm__ volatile ("sti");
}

void disable_int(void)
{
	__asm__ volatile ("cli");
}

void cpu_hlt(void)
{
	__asm__ volatile("hlt");
}

//String Operations
inline void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
	for (; len != 0; len--) {
		*dest++ = *src++;
	}
}

inline void memset(void *dest, uint8_t val, uint32_t len)
{
	uint8_t *dst = (uint8_t *)dest;

	for ( ; len != 0; len--) {
		*dst++ = val;
	}
}

inline void bzero(void *dest, uint32_t len)
{
	memset(dest, 0, len);
}

inline bool strcmp(const char *str1, const char *str2)
{
	while (*str1 && *str2 && (*str1++ == *str2++))
	      ;

	if (*str1 == '\0' && *str2 == '\0')
	{
	    return true;
	}

	if (*str1 == '\0') {
	    return false;
	}

	return true;
}

inline char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while (*src)
	{
	    *dest++ = *src++;
	}

	*dest = '\0';

	return tmp;
}

inline char *strcat(char *dest, const char *src)
{
	char *cp = dest;

	while (*cp) {
	      cp++;
	}

	while ((*cp++ = *src++))
	      ;

	return dest;
}

inline uint16_t strlen(const char *src)
{
	const char *eos = src;

        while (*eos++)
	      ;

	return (eos - src - 1);
}

