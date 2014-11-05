#pragma once

#include <cassert>
#include <string>

#include "ActionEffect.h"
#include "Player.h"

enum PlayerAbilityExecutionFailureReason {
	PLAYERABILITYEXECUTIONFAILUREREASON_NOFAILURE,
	PLAYERABILITYEXECUTIONFAILUREREASON_NOTENOUGHSTAMINA,
};

struct PlayerAbilityExecution {
	bool failure = false;
	PlayerAbilityExecutionFailureReason failure_reason = PLAYERABILITYEXECUTIONFAILUREREASON_NOFAILURE;
	ActionEffect effect_on_caster;
	ActionEffect effect_on_target;
};

class PlayerAbility {
	public:
	virtual std::string id() const = 0;
	virtual PlayerAbilityExecution execute(const Player& caster, const Player& target) const = 0;
	virtual std::string message(const PlayerAbilityExecution& execution, bool caster_is_you) const = 0;

	static std::unique_ptr<PlayerAbility> get_ability_by_id(const std::string& ability_id);
};

class AttackPlayerAbility : public PlayerAbility {
	public:
	std::string id() const { return "attack"; }

	PlayerAbilityExecution execute(const Player& caster, const Player& target) const {
		PlayerAbilityExecution effect;

		// check that the caster has enough stamina
		if (caster.stamina() < 4) {
			effect.failure = true;
			effect.failure_reason = PLAYERABILITYEXECUTIONFAILUREREASON_NOTENOUGHSTAMINA;
			return effect;
		}

		effect.effect_on_caster.effect_on_stamina(-4);
		effect.effect_on_target.deal_damage(6, target);

		return effect;
	}

	std::string message(const PlayerAbilityExecution& execution, bool caster_is_you) const {
		if (execution.failure) {
			if (execution.failure_reason == PLAYERABILITYEXECUTIONFAILUREREASON_NOTENOUGHSTAMINA) {
				return caster_is_you ? "Ability_Attack_NoStamina_You"
				                     : "Ability_Attack_NoStamina_Opponent";
			}
		} else {
			return caster_is_you ? "Ability_Attack_Execute_You" : "Ability_Attack_Execute_Opponent";
		}

		assert("fail" && false);
	}
};

class DefensePlayerAbility : public PlayerAbility {
	public:
	std::string id() const { return "defense"; }

	PlayerAbilityExecution execute(const Player& caster, const Player&) const {
		PlayerAbilityExecution effect;

		// check that the caster has enough stamina
		if (caster.stamina() < 2) {
			effect.failure = true;
			effect.failure_reason = PLAYERABILITYEXECUTIONFAILUREREASON_NOTENOUGHSTAMINA;
			return effect;
		}

		effect.effect_on_caster.effect_on_stamina(-2);
		effect.effect_on_caster.effect_on_defense(+2);

		return effect;
	}

	std::string message(const PlayerAbilityExecution& execution, bool caster_is_you) const {
		if (execution.failure) {
			if (execution.failure_reason == PLAYERABILITYEXECUTIONFAILUREREASON_NOTENOUGHSTAMINA) {
				return caster_is_you ? "Ability_Defend_NoStamina_You"
				                     : "Ability_Defend_NoStamina_Opponent";
			}
		} else {
			return caster_is_you ? "Ability_Defend_Execute_You" : "Ability_Defend_Execute_Opponent";
		}

		assert("fail" && false);
	}
};

class IdlePlayerAbility : public PlayerAbility {
	public:
	std::string id() const { return "idle"; }

	PlayerAbilityExecution execute(const Player&, const Player&) const {
		PlayerAbilityExecution effect;

		return effect;
	}

	std::string message(const PlayerAbilityExecution&, bool caster_is_you) const {
		return caster_is_you ? "Ability_Idle_Execute_You" : "Ability_Idle_Execute_Opponent";
	}
};

class ForfeitPlayerAbility : public PlayerAbility {
	public:
	std::string id() const { return "forfeit"; }

	PlayerAbilityExecution execute(const Player&, const Player&) const {
		PlayerAbilityExecution effect;

		return effect;
	}

	std::string message(const PlayerAbilityExecution&, bool caster_is_you) const {
		return caster_is_you ? "Ability_Forfeit_Execute_You" : "Ability_Forfeit_Execute_Opponent";
	}
};
