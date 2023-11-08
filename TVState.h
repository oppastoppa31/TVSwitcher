#ifndef TVSTATE
#define TVSTATE

#include <chrono>
#include <pthread.h>
#include <stdint.h>

/// @brief Pin number for the button
constexpr const uint8_t SWITCHER_PIN = 0;

/// @brief Pin number for the PIR sensor
constexpr const uint8_t PIR_PIN = 21;

/// @brief Camera motion timeout in seconds
constexpr const int32_t MOTION_TIMEOUT = 60 * 10;

using schedptr_t = void *(*)(void *args);

class TVState {
	public:
	explicit TVState(schedptr_t *const scheduler_thread_ptr);

	/// @brief Callback activated when motion starts or stops on the PIR
	/// sensor This will automatically switch to the the camera
	/// @param level The level of the GPIO Pin (0 or 1)
	void motion_callback(const int32_t level);

	/// @brief Schedule switches between the camera and the threat map
	/// @param args Arguments passed to the thread
	void scheduler_thread();

	private:
	enum class STATE {
		THREATMAP,
		CAMERA,
	} state;

	pthread_t scheduler_thread_id;
	schedptr_t *const scheduler_thread_ptr;

	/// @brief Switch between the camera and the threat map
	/// @param now The current time
	void switcher(const time_t now);
};

#endif /* TVSTATE */
