#include "buttons.h"

/**
 * @brief Function to read user button.
 */
bool button_read_user(void)
{
	return (bool)(gpio_get(GPIOC, GPIO13));
}
