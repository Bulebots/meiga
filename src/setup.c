#include "setup.h"

/**
 * @brief Initial clock setup.
 *
 * Use the Internal High Speed clock (HSI), at 16 MHz, and set the SYSCLK
 * at 168 MHz.
 *
 * The peripheral clocks are set to:
 *
 * - AHB to 168 MHz (max. is 180 MHz)
 * - APB1 to 42 MHz
 * - APB2 to 84 MHz
 *
 * Enable required clocks for the GPIOs and timers as well.
 *
 * @see Reference manual (RM0090), in particular "Reset and clock control for
 * STM32F405xx" section.
 */
static void setup_clock(void)
{
	rcc_clock_setup_hsi_3v3(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

	/* GPIOs */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Enable clock cycle counter */
	dwt_enable_cycle_counter();
}

/**
 * @brief Set SysTick interruptions frequency and enable SysTick counter.
 *
 * SYSCLK is at 168 MHz as well as the Advanced High-permormance Bus (AHB)
 * because, by default, the AHB divider is set to 1, so the AHB clock has the
 * same frequency as the SYSCLK.
 *
 * SysTick interruption frequency is set to `SYSTICK_FREQUENCY_HZ`.
 *
 * @see RM0090 reference manual and in particular the "Clock tree" figure.
 */
static void setup_systick(void)
{
	systick_set_frequency(SYSTICK_FREQUENCY_HZ, SYSCLK_FREQUENCY_HZ);
	systick_counter_enable();
}

/**
 * @brief Enable SysTick interruption.
 */
void enable_systick_interruption(void)
{
	systick_interrupt_enable();
}

/**
 * @brief Disable SysTick interruption.
 */
void disable_systick_interruption(void)
{
	systick_interrupt_disable();
}

/**
 * @brief Initial GPIO configuration.
 *
 * Set GPIO modes and initial states.
 */
static void setup_gpio(void)
{
	/* LEDs */
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
			GPIO0 | GPIO1 | GPIO2 | GPIO3);
	gpio_clear(GPIOA, GPIO0 | GPIO1 | GPIO2 | GPIO3);
}

/**
 * @brief Execute all setup functions.
 */
void setup(void)
{
	setup_clock();
	setup_gpio();
	setup_systick();
}
