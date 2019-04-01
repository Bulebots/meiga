#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdlib.h>
#include <string.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/sync.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/usart.h>

bool serial_acquire_transfer_lock(void);
void serial_send(char *data, int size);

#endif /* __SERIAL_H */
