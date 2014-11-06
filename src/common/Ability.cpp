#include <memory>
#include <string>
#include <map>
#include <functional>

#include "Ability.h"

// clang-format off
#define DECLARE_ABILITY(x) { x::id(), []() { return std::unique_ptr<PlayerAbility>(new x()); } }

static std::map<std::string, std::function<std::unique_ptr<PlayerAbility>()>> abilities = {
	DECLARE_ABILITY(SlashPlayerAbility),
	DECLARE_ABILITY(DefendPlayerAbility),
	DECLARE_ABILITY(IdlePlayerAbility),
	DECLARE_ABILITY(ForfeitPlayerAbility),
};
// clang-format on

std::unique_ptr<PlayerAbility> PlayerAbility::get_ability_by_id(const std::string& ability_id) {
	if (abilities.count(ability_id) == 1) {
		return abilities[ability_id]();
	}

	return std::unique_ptr<PlayerAbility>(nullptr);
}
