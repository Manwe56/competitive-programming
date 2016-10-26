#include "gtest/gtest.h"

#include "competitive/programming/timemanagement/Timer.hpp"

#include <thread>

using competitive::programming::timemanagement::Timer;
using competitive::programming::timemanagement::TimeoutException;

static void sleep(std::chrono::milliseconds sleep) {
	std::this_thread::sleep_for(sleep);
}

TEST(Timer, NonStartedTimerDoesNotTimeOut)
{
	Timer timer;

	try {
		timer.timeCheck();
		sleep(std::chrono::milliseconds(1));
		timer.timeCheck();
	}
	catch (TimeoutException& e) {
		FAIL();
	}
}

TEST(Timer, DoesNotTimeoutBeforeTimeoutReached) {
	Timer timer;
	try {
		timer.startTimer(2.0);
		timer.timeCheck();
	}
	catch (TimeoutException& e) {
		FAIL();
	}
}

TEST(Timer, timeOutReached) {
	Timer timer;
	timer.startTimer(1.0);
	sleep(std::chrono::milliseconds(2));
	ASSERT_THROW(timer.timeCheck(), TimeoutException);
}
