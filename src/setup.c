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
 * A pull-up resistor is used in RX to avoid a floating input when no
 * bluetooth is connected, which could trigger incorrect interruptions.
 *
 * @see Reference manual (RM0090), in particular "Reset and clock control for
 * STM32F405xx" section.
 */
static void setup_clock(void)
{
	rcc_clock_setup_hsi_3v3(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

	/* GPIOs */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Bluetooth */
	rcc_periph_clock_enable(RCC_USART1);

	/* Timers */
	rcc_periph_clock_enable(RCC_TIM3);
	rcc_periph_clock_enable(RCC_TIM4);
	rcc_periph_clock_enable(RCC_TIM8);
	rcc_periph_clock_enable(RCC_TIM11);

	/* ADC */
	rcc_periph_clock_enable(RCC_ADC2);

	/* DMA */
	rcc_periph_clock_enable(RCC_DMA2);

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
 * @brief Setup for ADC2: configured for regular conversion.
 *
 * This ADC is used to read the battery status.
 *
 * - Power off the ADC to be sure that does not run during configuration
 * - Disable scan mode
 * - Set single conversion mode triggered by software
 * - Configure the alignment (right)
 * - Configure the sample time (15 cycles of ADC clock)
 * - Set regular sequence with `channel_sequence` structure
 * - Power on the ADC
 *
 * @see Reference manual (RM0090) "Analog-to-digital converter".
 */
static void setup_adc2(void)
{
	uint8_t channel_sequence[16];

	channel_sequence[0] = ADC_CHANNEL14;
	adc_power_off(ADC2);
	adc_disable_scan_mode(ADC2);
	adc_set_single_conversion_mode(ADC2);
	adc_set_right_aligned(ADC2);
	adc_set_sample_time_on_all_channels(ADC2, ADC_SMPR_SMP_15CYC);
	adc_set_regular_sequence(ADC2, 1, channel_sequence);
	adc_power_on(ADC2);
}

/**
 * @brief Initial GPIO configuration.
 *
 * Set GPIO modes and initial states.
 *
 * @see STM32F405RG datasheet and in particular the "Alternate function
 * mapping" section.
 */
static void setup_gpio(void)
{
	/* Battery */
	gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO4);

	/* Infrared emitters */
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
			GPIO4 | GPIO5 | GPIO6 | GPIO7);
	gpio_clear(GPIOA, GPIO4 | GPIO5 | GPIO6 | GPIO7);

	/* LEDs */
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
			GPIO0 | GPIO1 | GPIO2 | GPIO3);
	gpio_clear(GPIOA, GPIO0 | GPIO1 | GPIO2 | GPIO3);

	/* Speaker */
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
	gpio_set_af(GPIOB, GPIO_AF3, GPIO9);

	/* Motor driver */
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE,
			GPIO6 | GPIO7 | GPIO8 | GPIO9);
	gpio_set_af(GPIOC, GPIO_AF3, GPIO6 | GPIO7 | GPIO8 | GPIO9);

        /* Encoders */
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE,
                        GPIO4 | GPIO5 | GPIO6 | GPIO7);
	gpio_set_af(GPIOB, GPIO_AF2, GPIO4 | GPIO5 | GPIO6 | GPIO7);

	/* Bluetooth */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9 | GPIO10);
	gpio_set_af(GPIOA, GPIO_AF7, GPIO9 | GPIO10);
	gpio_set(GPIOA, GPIO10);

	/* Buttons */
	gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO13);
}

/**
 * @brief Setup USART for bluetooth communication.
 */
static void setup_usart(void)
{
	usart_set_baudrate(USART1, 921600);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX_RX);

	usart_enable(USART1);
}

/**
 * @brief Setup PWM for the motor drivers.
 *
 * TIM8 is used to generate both PWM signals (left and right motor):
 *
 * - Edge-aligned, up-counting timer.
 * - Prescale to increment timer counter at 24 MHz.
 * - Set PWM frequency to 24 kHz.
 * - Configure channels 1, 2, 3 and 4 as output GPIOs.
 * - Set output compare mode to PWM1 (output is active when the counter is
 *   less than the compare register contents and inactive otherwise.
 * - Reset output compare value (set it to 0).
 * - Enable channels 1, 2, 3 and 4 outputs.
 * - Enable outputs in the break subsystem (required on an advanced timer).
 * - Enable timer counter.
 *
 * @see Reference manual (RM0090) "Advanced-control timers (TIM1 and TIM8)"
 * and in particular the "PWM mode" section.
 */
static void setup_motor_driver(void)
{
	timer_set_mode(TIM8, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
		       TIM_CR1_DIR_UP);

	timer_set_prescaler(
	    TIM8, (rcc_apb2_frequency / 24000000 - 1));
	timer_set_repetition_counter(TIM8, 0);
	timer_enable_preload(TIM8);
	timer_continuous_mode(TIM8);
	timer_set_period(TIM8, DRIVER_PWM_PERIOD);

	timer_set_oc_mode(TIM8, TIM_OC1, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM8, TIM_OC2, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM8, TIM_OC3, TIM_OCM_PWM1);
	timer_set_oc_mode(TIM8, TIM_OC4, TIM_OCM_PWM1);
	timer_set_oc_value(TIM8, TIM_OC1, 0);
	timer_set_oc_value(TIM8, TIM_OC2, 0);
	timer_set_oc_value(TIM8, TIM_OC3, 0);
	timer_set_oc_value(TIM8, TIM_OC4, 0);
	timer_enable_oc_output(TIM8, TIM_OC1);
	timer_enable_oc_output(TIM8, TIM_OC2);
	timer_enable_oc_output(TIM8, TIM_OC3);
	timer_enable_oc_output(TIM8, TIM_OC4);

	timer_enable_break_main_output(TIM8);

	timer_enable_counter(TIM8);
}

/**
 * @brief Configure timer to read a quadrature encoder.
 *
 * - Set the Auto-Reload Register (TIMx_ARR).
 * - Set the encoder interface mode counting on both TI1 and TI2 edges.
 * - Configure inputs (see note).
 * - Enable counter.
 *
 * @param[in] timer_peripheral Timer register address base to configure.
 *
 * @note It currently always uses channels 1 and 2.
 *
 * @see Reference manual (RM0090) "TIM2 to TIM5 functional description" and in
 * particular "Encoder interface mode" section.
 */
static void configure_timer_as_quadrature_encoder(uint32_t timer_peripheral)
{
	timer_set_period(timer_peripheral, 0xFFFF);
	timer_slave_set_mode(timer_peripheral, 0x3);
	timer_ic_set_input(timer_peripheral, TIM_IC1, TIM_IC_IN_TI1);
	timer_ic_set_input(timer_peripheral, TIM_IC2, TIM_IC_IN_TI2);
	timer_enable_counter(timer_peripheral);
}

/**
 * @brief Setup timers for the motor encoders.
 *
 * TIM3 for the left motor and TIM4 for the right motor are configured.
 */
static void setup_encoders(void)
{
	configure_timer_as_quadrature_encoder(TIM3);
	configure_timer_as_quadrature_encoder(TIM4);
}

/**
 * @brief Setup PWM for the speaker.
 *
 * TIM11 is used to generate the PWM signals for the speaker:
 *
 * - Edge-aligned, up-counting timer.
 * - Prescale to increment timer counter at SPEAKER_BASE_FREQUENCY_HZ.
 * - Set output compare mode to PWM1 (output is active when the counter is
 *   less than the compare register contents and inactive otherwise.
 * - Disable output compare output (speaker is off by default).
 *
 * @see Reference manual (RM0090) "General-purpose timers (TIM9 to TIM14)"
 * and in particular the "PWM mode" section.
 */
static void setup_speaker(void)
{
	timer_set_mode(TIM11, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE,
		       TIM_CR1_DIR_UP);

	timer_set_prescaler(
	    TIM11, (rcc_apb2_frequency / SPEAKER_BASE_FREQUENCY_HZ - 1));
	timer_set_repetition_counter(TIM11, 0);
	timer_enable_preload(TIM11);
	timer_continuous_mode(TIM11);

	timer_disable_oc_output(TIM11, TIM_OC1);
	timer_set_oc_mode(TIM11, TIM_OC1, TIM_OCM_PWM1);
}

/**
 * @brief Execute all setup functions.
 */
void setup(void)
{
	setup_clock();
	setup_gpio();
	setup_speaker();
	setup_motor_driver();
	setup_encoders();
	setup_usart();
	setup_adc2();
	setup_systick();
}
