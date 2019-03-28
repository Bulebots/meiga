#include "mmlib/clock.h"

#include "setup.h"

/**
 * @brief Handle the SysTick interruptions.
 */
void sys_tick_handler(void)
{
	clock_tick();
}

/**
 * @brief Initial setup and infinite wait.
 */
int main(void)
{
	setup();
	systick_interrupt_enable();
	return 0;
}
