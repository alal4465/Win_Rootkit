#pragma once
#include "common.h"
#include <winuser.h>
#include <fstream>
#include <atomic>
namespace Logger {

	constexpr char FILE_NAME[] = "log.txt";
	extern std::atomic<bool> log_running;
	void logger_thread();

}