#include <iostream>
#include <stdint.h>
#include <wiringPi.h>

/// @brief Pin number for the button
constexpr const uint8_t BUTTON_PIN = 0;

/// @brief Camera motion timeout in seconds
constexpr const uint32_t MOTION_TIMEOUT = 60 * 10;

/// @brief Setup the program
void setup() {
	wiringPiSetup();
	pinMode(BUTTON_PIN, OUTPUT);
}

int main(int argc, char const *argv[]) {
	setup();
	/* code */
	return 0;
}
