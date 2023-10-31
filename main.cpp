#include <chrono>
#include <iostream>
#include <pigpio.h>
#include <stdint.h>
#include <thread>

using namespace std;
using namespace this_thread;
using namespace chrono_literals;

/// @brief Pin number for the button
constexpr const uint8_t SWITCHER_PIN = 0;

/// @brief Pin number for the PIR sensor
constexpr const uint8_t PIR_PIN = 12;

/// @brief Camera motion timeout in seconds
constexpr const uint32_t MOTION_TIMEOUT = 60 * 10;

/// @brief Setup the program
static void setup() {
	if (gpioInitialise() < 0) {
		cerr << "pigpio initialisation failed" << endl;
		exit(EXIT_FAILURE);
	}
	gpioSetMode(SWITCHER_PIN, PI_OUTPUT);
	gpioSetMode(PIR_PIN, PI_INPUT);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
	setup();
	while (true) {
		cout << gpioRead(PIR_PIN) << endl;
		sleep_for(1s);
	}
	return EXIT_SUCCESS;
}
