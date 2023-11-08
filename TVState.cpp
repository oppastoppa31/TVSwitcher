#include "TVState.h"
#include <chrono>
#include <iostream>
#include <thread>

TVState::TVState(schedptr_t *const scheduler_thread_ptr):
	state(TVState::STATE::CAMERA),
	scheduler_thread_ptr(scheduler_thread_ptr) {}

using system_clock = std::chrono::system_clock;
using time_point   = std::chrono::time_point<system_clock>;

void TVState::motion_callback(const int32_t level) {
	const time_t now = system_clock::to_time_t(system_clock::now());
	if (level == 1) {
		// Motion started
		std::cout << "Motion detected at " << ctime(&now) << std::endl;
		// Switch to camera
		if (this->state == TVState::STATE::THREATMAP) { this->switcher(now); }
	} else {
		// Motion stopped
		std::cout << "Motion stopped at " << ctime(&now) << std::endl;
		// If there is already a queued switch, cancel it
		if (this->scheduler_thread_id != 0) {
			pthread_cancel(this->scheduler_thread_id);
			this->scheduler_thread_id = 0;
		}
		// Schedule a switch to the threat map
		if (pthread_create(
				&this->scheduler_thread_id, nullptr,
				*this->scheduler_thread_ptr, nullptr) != 0) {
			std::cerr << "Failed to create thread" << std::endl;
		}
	}
}

void TVState::scheduler_thread() {
	if (this->state == TVState::STATE::CAMERA) {
		// Sleep for MOTION_TIMEOUT seconds (we will be canceled if motion
		// happens again)
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		pthread_cond_t cond	  = PTHREAD_COND_INITIALIZER;
		const time_point now  = system_clock::now();
		pthread_mutex_lock(&mutex);
		timespec ts;
		ts.tv_sec = system_clock::to_time_t(now) +
			std::chrono::seconds(MOTION_TIMEOUT).count();
		ts.tv_nsec = now.time_since_epoch().count() % 1'000'000'000;
		pthread_cond_timedwait(&cond, &mutex, &ts);
		pthread_mutex_unlock(&mutex);
		this->switcher(system_clock::to_time_t(system_clock::now()));
	}
}

void TVState::switcher(const time_t now) {
	if (this->state == TVState::STATE::CAMERA) {
		std::cout << "Switching to threat map at " << ctime(&now) << std::endl;
		// gpioWrite(SWITCHER_PIN, 1);
		// sleep_for(0.1s);
		// gpioWrite(SWITCHER_PIN, 0);
		this->state = TVState::STATE::THREATMAP;
	} else {
		std::cout << "Switching to camera at " << ctime(&now) << std::endl;
		// gpioWrite(SWITCHER_PIN, 1);
		// sleep_for(0.1s);
		// gpioWrite(SWITCHER_PIN, 0);
		this->state				  = TVState::STATE::CAMERA;
		this->scheduler_thread_id = 0;
	}
}
