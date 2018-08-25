#ifndef _LIB_H_
#define _LIB_H_

#define sizeof(x) ((char*)(&(x) + 1) - (char*)&(x))  //will be expanded later and should not()
#define SIZEOF(x) ({	\
	(x) this_type_instance;	\
	(char*)(&this_type_instance + 1) - (char*)&this_type_instance;	\
})
#define is_digit(x) ((x) >= '0' && (x) <= '9')

void outb(uint16_t port_idx, uint8_t write_value);

uint8_t inb(uint16_t port_idx);
uint16_t inw(uint16_t port_idx);

void insl(uint32_t port, void * addr, uint32_t n);
void outsl(uint32_t port, void * addr, uint32_t n);

void enable_int(void);
void disable_int(void);
void cpu_hlt(void);

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);
void memset(void *dest, uint8_t val, uint32_t len);

void bzero(void *dest, uint32_t len);

bool strcmp(const char *str1, const char *str2);
char *strcpy(char *dest, const char *src);
char *strcat(char *dest, const char *src);

uint16_t strlen(const char *src);

#endif