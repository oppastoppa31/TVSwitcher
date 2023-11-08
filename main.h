#ifndef MAIN
#define MAIN
#include <stdint.h>

static void *scheduler_thread([[maybe_unused]] void *args);
static void motion_callback(
	[[maybe_unused]] const int32_t gpio, [[maybe_unused]] const int32_t level,
	[[maybe_unused]] const uint32_t tick);
#endif /* MAIN */
