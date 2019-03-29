#ifndef __SETUP_H
#define __SETUP_H

#include <libopencm3/cm3/dwt.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/usart.h>

#include "mylibopencm3.h"

/** Universal constants */
#define MICROMETERS_PER_METER 1000000
#define MICROSECONDS_PER_SECOND 1000000
#define PI 3.1415

/** System clock frequency is set in `setup_clock` */
#define SYSCLK_FREQUENCY_HZ 168000000
#define SPEAKER_BASE_FREQUENCY_HZ 1000000
#define SYSTICK_FREQUENCY_HZ 1000

void setup(void);
void enable_systick_interruption(void);
void disable_systick_interruption(void);

#endif /* __SETUP_H */
