#include "../include/baremetal.h"

void spin_delay (volatile u32 ticks) {
	while(ticks--);
}

int main() {
	RCC_config->AHB1_peripheral_enable = 0
		| RCC_AHB1_PERIPHERAL_ENABLE__GPIOD
		;

	GPIO_config_D->mode = 0
		| (GPIO_MODE__OUTPUT * (GPIO_WPIN_15 | GPIO_WPIN_14 | GPIO_WPIN_13 | GPIO_WPIN_12))
		| (GPIO_MODE__INPUT  * (GPIO_WPIN_00))
		;

	GPIO_config_D->pull_type = 0
		| (GPIO_PULL_TYPE__UP * GPIO_WPIN_00)
		;

	GPIO_config_D->output_speed = 0
		| (GPIO_OUTPUT_SPEED__LOW * (GPIO_WPIN_15 | GPIO_WPIN_14 | GPIO_WPIN_13 | GPIO_WPIN_12 | GPIO_WPIN_00))
		;

	u32 set_pin[4] = {GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15};
	u32 off = 1;
	bool was_pressed = !!(GPIO_config_D->input_data & GPIO_WPIN_00);

	for (u32 j = 0; j < 4; ++j) {
		GPIO_config_D->bit_set_reset = 0
			| GPIO_BIT_SET_RESET__RESET(set_pin[(j + off) & 3])
			| GPIO_BIT_SET_RESET__SET(set_pin[j & 3])
			;
	}

	for (u32 i = 0; ; ++i) {
		for (u32 count = 0; count < 10; ++count) {
			bool is_pressed = !!(GPIO_config_D->input_data & GPIO_WPIN_00);
			if (is_pressed && !was_pressed) {
				off = (off + 1) & 3;
				for (u32 j = 0; j < 4; ++j) {
					GPIO_config_D->bit_set_reset = 0
						| GPIO_BIT_SET_RESET__RESET(set_pin[(i + j + off) & 3])
						| GPIO_BIT_SET_RESET__SET(set_pin[(i + j) & 3])
						;
				}
			}
			was_pressed = is_pressed;
			spin_delay(25000);
		}
		GPIO_config_D->bit_set_reset = 0
			| GPIO_BIT_SET_RESET__RESET(set_pin[(i + off) & 3])
			| GPIO_BIT_SET_RESET__SET(set_pin[i & 3])
			;
	}
}
