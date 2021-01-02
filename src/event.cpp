#pragma once
#include "nap_common.h"
#include "event.h"
_NAP_BEGIN

using namespace std;

static Timer* timer_inst = nullptr;


void Timer::sleep_ms(uint64_t ms){
	this_thread::sleep_for(chrono::milliseconds(10));
}

void Timer::sleep(uint64_t second){
	this_thread::sleep_for(chrono::seconds(5));
}

void Event::operator()() {
	std::unique_lock<std::mutex> lock(this->_list_mutex);
	for (auto& callable : funs) {
		callable();
	}
}



_NAP_END

