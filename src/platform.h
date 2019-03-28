#ifndef __PLATFORM_H
#define __PLATFORM_H

#include <stdint.h>

#include <libopencm3/cm3/dwt.h>

uint32_t read_cycle_counter(void);

#endif /* __PLATFORM_H */
