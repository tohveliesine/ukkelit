#pragma once

#include <format.h>
#include <string>

class Translations {

	public:
	Translations();
	~Translations();

	template <typename... Args> std::string format(const std::string& key, Args... args) const {
		return fmt::format(get(key), args...);
	}

	std::string get(const std::string& key) const;
};

// get the translation for the given key and parameters
template <typename... Args> std::string t(const std::string& key, Args... args) {
	static const Translations translations;
	return translations.format(key, args...);
}