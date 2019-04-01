#include "serial.h"

static mutex_t _send_lock;

/**
 * @brief Try to acquire the serial transfer lock.
 *
 * @return Whether the lock was acquired or not.
 */
bool serial_acquire_transfer_lock(void)
{
	return (bool)mutex_trylock(&_send_lock);
}

/**
 * @brief Send data through serial.
 *
 * DMA is configured to read from `data` a number `size` of bytes. It then
 * writes all those bytes to USART1 (Bluetooth).
 *
 * An interruption is generated when the transfer is complete.
 *
 * @param[in] data Data to send.
 * @param[in] size Size (number of bytes) to send.
 */
void serial_send(char *data, int size)
{
	dma_stream_reset(DMA2, DMA_STREAM7);

	dma_enable_memory_increment_mode(DMA2, DMA_STREAM7);
	dma_set_peripheral_size(DMA2, DMA_STREAM7, DMA_SxCR_PSIZE_8BIT);
	dma_set_memory_size(DMA2, DMA_STREAM7, DMA_SxCR_MSIZE_8BIT);
	dma_set_priority(DMA2, DMA_STREAM7, DMA_SxCR_PL_VERY_HIGH);
	dma_set_transfer_mode(DMA2, DMA_STREAM7,
			      DMA_SxCR_DIR_MEM_TO_PERIPHERAL);

	dma_set_peripheral_address(DMA2, DMA_STREAM7, (uint32_t)&USART1_DR);
	dma_set_memory_address(DMA2, DMA_STREAM7, (uint32_t)data);
	dma_set_number_of_data(DMA2, DMA_STREAM7, size);

	dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM7);
	dma_channel_select(DMA2, DMA_STREAM7, DMA_SxCR_CHSEL_4);
	dma_enable_stream(DMA2, DMA_STREAM7);
	usart_enable_tx_dma(USART1);
}

/**
 * @brief DMA 2 stream 7 interruption routine.
 *
 * Executed on serial transfer complete. Clears the interruption flag, and
 * disables serial transfer DMA until next call to `serial_send()`.
 *
 * It will also release the serial transfer lock.
 */
void dma2_stream7_isr(void)
{
	if (dma_get_interrupt_flag(DMA2, DMA_STREAM7, DMA_TCIF))
		dma_clear_interrupt_flags(DMA2, DMA_STREAM7, DMA_TCIF);

	dma_disable_transfer_complete_interrupt(DMA2, DMA_STREAM7);
	usart_disable_tx_dma(USART1);
	dma_disable_stream(DMA2, DMA_STREAM7);
	mutex_unlock(&_send_lock);
}
