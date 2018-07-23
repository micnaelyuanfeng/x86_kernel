#include "types.h"
#include "color.h"
#include "display.h"
#include "list.h"

inline void find_fit(uint32_t size)
{
	uint16_t counter = 0;

	whie(size)
	{
		if(size > 32)
		{
			size = size >> 3;
			counter++;
		}
		else
		{
			size = size >> 1;
			counter++;
		}
	}

	if(counter >= 12)
	{
		return 11;
	}
	else
	{
		return -1;
	}
}

