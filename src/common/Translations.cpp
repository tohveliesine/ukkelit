#include <string>
#include <map>

#include "Translations.h"

// clang-format off
static std::map<std::string, std::string> translations = {
    {"Startup_Name", "Ukkelit v{0}"}, // {0} is the version number, e.g. 0.1
    {"Startup_Intro", "A 1 vs 1 strategic hero combat game."},
};
// clang-format on

Translations::Translations() {}

Translations::~Translations() {}

std::string Translations::get(const std::string& key) const {
	if (translations.count(key) == 0) {
		return "[*" + key + "]";
	}

	return translations[key];
}