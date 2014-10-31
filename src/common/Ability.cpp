#include <memory>
#include <string>
#include <map>
#include <functional>

#include "Ability.h"

static std::map<std::string, std::function<std::unique_ptr<PlayerAbility>()>> abilities = {
		{ "attack", []() {return std::unique_ptr<PlayerAbility>(new AttackPlayerAbility()); } },
		{ "defense", []() {return std::unique_ptr<PlayerAbility>(new DefensePlayerAbility()); } },
		{ "idle", []() {return std::unique_ptr<PlayerAbility>(new IdlePlayerAbility()); } },
		{ "forfeit", []() {return std::unique_ptr<PlayerAbility>(new ForfeitPlayerAbility()); } },
};

std::unique_ptr<PlayerAbility> PlayerAbility::get_ability_by_id(const std::string& ability_id) {
	if (abilities.count(ability_id) == 1) {
		return abilities[ability_id]();
	}

	return std::unique_ptr<PlayerAbility>(nullptr);
}