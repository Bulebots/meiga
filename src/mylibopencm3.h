#ifndef __MYLIBOPENCM3_H
#define __MYLIBOPENCM3_H

#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/rcc.h>

void rcc_clock_setup_hsi_3v3(const struct rcc_clock_scale *clock);

#endif /* __MYLIBOPENCM3_H */
