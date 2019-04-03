#include "platform.h"

/**
 * @brief Read the microcontroller clock cycle counter.
 *
 * This counter increases by one at `SYSCLK_FREQUENCY_HZ`.
 */
uint32_t read_cycle_counter(void)
{
	return dwt_read_cycle_counter();
}

/**
 * @brief Read left motor encoder counter.
 */
uint16_t read_encoder_left(void)
{
	return (uint16_t)timer_get_counter(TIM3);
}

/**
 * @brief Read right motor encoder counter.
 */
uint16_t read_encoder_right(void)
{
	return (uint16_t)timer_get_counter(TIM4);
}

/**
 * @brief Function to get battery voltage.
 *
 * The value is converted from bits to voltage taking into account that the
 * battery voltage is read through a voltage divider.
 *
 *@return The battery voltage in volts.
 */
float get_battery_voltage(void)
{
	uint16_t battery_bits;

	adc_start_conversion_regular(ADC2);
	while (!adc_eoc(ADC2))
		;
	battery_bits = adc_read_regular(ADC2);
	return battery_bits * ADC_LSB * VOLT_DIV_FACTOR;
}

/**
 * @brief Function to get the motors voltage.
 *
 * In Meiga, this function simply returns the value of the battery voltage.
 *
 *@return The motors voltage in volts.
 */
float get_motors_voltage(void)
{
	return get_battery_voltage();
}

/**
 * @brief Turn on the speaker to play at the selected frequency.
 *
 * Frequency is set modulating the PWM signal sent to the speaker.
 *
 * @param[in] hz Frequency, in Hertz.
 */
void speaker_on(float hz)
{
	uint16_t period;

	period = (uint16_t)(SPEAKER_BASE_FREQUENCY_HZ / hz);
	timer_set_period(TIM11, period);
	timer_set_oc_value(TIM11, TIM_OC1, period / 2);
	timer_enable_counter(TIM11);
	timer_enable_oc_output(TIM11, TIM_OC1);
}

/**
 * @brief Turn off the speaker.
 */
void speaker_off(void)
{
	timer_disable_counter(TIM11);
	timer_disable_oc_output(TIM11, TIM_OC1);
}
