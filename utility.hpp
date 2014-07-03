#pragma once
#include <utility>
#include <stdexcept>
#include <iostream>

namespace utility {

template <typename T>
class optional
	{
	public:
	const bool present;
	private:
	T value;
	optional(bool present, T value) : present(present), value(value){}
	optional(bool present) : present(present) {}
	public:
	optional(optional&& old) : present(old.present), value(old.value) {}
	static optional just(T&& value)
		{
		return optional(true,value);
		}
	static optional nothing()
		{
		return optional(false);
		}
	
	T& get_value()
		{
		if (present)
			return value;
		else
			throw std::runtime_error("Value does not exist");
		}
	};

enum class log_level
	{
	EMERG, // The program is about to crash
	ALERT, // Something bad happened and action must be taken immediately
	CRIT, // A critical condition like a serious software failure
	ERR, // An error condition
	WARNING, // Nothing serious by itself by might indicate problems
	NOTICE, // Nothing serious, but notable nevertheless.
	INFO, // Informational messages
	DEBUG, // Debug messages
	};

log_level current_log_level;
void set_log_level(log_level target)
	{
	current_log_level = target;
	}

log_level get_log_level()
	{
	return current_log_level;
	}

void log(std::string msg, log_level level=log_level::DEBUG, std::ostream& os = std::cerr) {
	if (level <= get_log_level()) {
		if (level == log_level::WARNING) os << "WARNING: ";
		if (level <= log_level::ERR) os << "ERROR: ";
		os << msg << std::endl;
		}
	}

};
