#include "main.h"
#include "TVState.h"
#include <iostream>
#include <pigpio.h>
#include <pthread.h>
#include <stddef.h>

schedptr_t scheduler_thread_ptr = scheduler_thread;
static TVState *state			= new TVState(&scheduler_thread_ptr);

static void *scheduler_thread([[maybe_unused]] void *args) {
	state->scheduler_thread();
	return nullptr;
}

static void motion_callback(
	[[maybe_unused]] const int32_t gpio, [[maybe_unused]] const int32_t level,
	[[maybe_unused]] const uint32_t tick) {
	state->motion_callback(level);
}

/// @brief Setup the program
static uint8_t setup() {
	if (gpioInitialise() < 0) {
		std::cerr << "pigpio init failed" << std::endl;
		gpioTerminate();
		return EXIT_FAILURE;
	}
	gpioSetMode(SWITCHER_PIN, PI_OUTPUT);
	gpioSetMode(PIR_PIN, PI_INPUT);
	gpioSetAlertFunc(PIR_PIN, motion_callback);
	return EXIT_SUCCESS;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
	if (setup() == EXIT_FAILURE) { return EXIT_FAILURE; }
	while (true) {}
	return EXIT_SUCCESS;
}
