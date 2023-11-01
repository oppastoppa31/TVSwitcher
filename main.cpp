#include <bitset>
#include <chrono>
#include <iostream>
#include <pigpio.h>
#include <stdint.h>
#include <thread>

using namespace std;
using namespace this_thread;
using namespace chrono_literals;
using namespace chrono;

/// @brief Pin number for the button
constexpr const uint8_t SWITCHER_PIN = 0;

/// @brief Pin number for the PIR sensor
constexpr const uint8_t PIR_PIN = 21;

/// @brief Camera motion timeout in seconds
constexpr const int32_t MOTION_TIMEOUT = 60 * 10;

typedef struct {
	enum class state {
		THREATMAP,
		CAMERA,
	} state;
	time_t last_motion;
} state_t;

state_t state = {};

/// @brief Switch the TV state
static void switcher() {
	if (state.state == state_t::state::CAMERA) {
		const time_t now = system_clock::to_time_t(system_clock::now());
		if (now - state.last_motion < MOTION_TIMEOUT) { return; }
		cout << "Switching to threat map at " << ctime(&now) << endl;
		// gpioWrite(SWITCHER_PIN, 1);
		// sleep_for(0.1s);
		// gpioWrite(SWITCHER_PIN, 0);
		state.state		  = state_t::state::THREATMAP;
		state.last_motion = 0;
	} else {
		const time_t now = system_clock::to_time_t(system_clock::now());
		cout << "Switching to camera at " << ctime(&now) << endl;
		// gpioWrite(SWITCHER_PIN, 1);
		// sleep_for(0.1s);
		// gpioWrite(SWITCHER_PIN, 0);
		state.state		  = state_t::state::CAMERA;
		state.last_motion = now;
	}
}

/// @brief Callback activated when motion starts or stops on the PIR sensor
/// @param gpio The GPIO Pin triggering the callback
/// @param level The level of the GPIO Pin (0 or 1)
/// @param tick The tick count when the callback was triggered in microseconds
static void motion_callback(
	[[maybe_unused]] const int32_t gpio, [[maybe_unused]] const int32_t level,
	[[maybe_unused]] const uint32_t tick) {
	const time_t now = system_clock::to_time_t(system_clock::now());
	if (level == 1) {
		// Motion started
		cout << "Motion detected at " << ctime(&now) << endl;
		if (state.state == state_t::state::THREATMAP) { switcher(); }
	} else {
		// Motion stopped
		cout << "Motion stopped at " << ctime(&now) << endl;
		state.last_motion = now;
	}
}

/// @brief Setup the program
static void setup() {
	if (gpioInitialise() < 0) {
		cerr << "pigpio init failed" << endl;
		gpioTerminate();
		exit(EXIT_FAILURE);
	}
	gpioSetMode(SWITCHER_PIN, PI_OUTPUT);
	gpioSetMode(PIR_PIN, PI_INPUT);
	gpioSetAlertFunc(PIR_PIN, motion_callback);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[]) {
	setup();
	while (true) {
		//	bool motion = gpioRead(PIR_PIN);
		if (state.state == state_t::state::CAMERA) { switcher(); }
		sleep_for(0.1s);
	}
	return EXIT_SUCCESS;
}
