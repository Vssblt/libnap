#pragma once
#include "nap_common.h"
#include "binstream.h"
_NAP_BEGIN

class Timer {
public:
	
	static void sleep_ms(uint64_t milliseconds);
	static void sleep(uint64_t second);


private:
	Timer();


};


class Event {
public:


	template<class Fun, class... Args>
	bool add(Fun&& f, Args&&... args);

	template<class Fun>
	bool remove(Fun&& f);

	void operator()();

//
//private:

	std::mutex _list_mutex;
	std::list<std::function<void()>> funs;
	std::list<size_t> funs_hash;


};


template<class Fun, class ...Args>
inline bool Event::add(Fun&& f, Args && ...args){
	std::unique_lock<std::mutex> lock(this->_list_mutex);
	auto fun = std::bind(
			std::forward<Fun>(f), 
			std::forward<Args>(args)...
		);
	funs.emplace_back(fun);
	funs_hash.push_back(typeid(f).hash_code());
	return true;
}

template<class Fun>
inline bool Event::remove(Fun&& f){
	bool flag = false;

	std::unique_lock<std::mutex> lock(this->_list_mutex);

	auto itor = this->funs.begin();
	auto itor_hash = this->funs_hash.begin();

	size_t pamhash = typeid(f).hash_code();

	while (itor != this->funs.end()) {
		size_t hash = *itor_hash;
		if (hash == pamhash) {
			itor = this->funs.erase(itor);
			itor_hash = this->funs_hash.erase(itor_hash);
			flag = true;
		}else {
			*itor++;
			*itor_hash++;
		}
	}
	return flag;
}


_NAP_END