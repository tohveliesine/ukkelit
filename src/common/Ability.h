#pragma once

#include <cassert>
#include <string>
#include <vector>

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
	virtual std::vector<std::string> commands() const = 0;
	virtual std::string ability_description_key() const = 0;
	virtual std::string ability_id() const = 0;
	virtual PlayerAbilityExecution execute(const Player& caster, const Player& target) const = 0;
	virtual std::string message(const PlayerAbilityExecution& execution, bool caster_is_you) const = 0;

	static std::unique_ptr<PlayerAbility> get_ability_by_id(const std::string& ability_id);
};

class SlashPlayerAbility : public PlayerAbility {
	public:
	static std::string id() { return "slash"; }
	std::string ability_id() const { return id(); }

	std::vector<std::string> commands() const { return {"attack", "slash"}; }
	std::string ability_description_key() const { return "Ability_Slash_Description"; }

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
				return caster_is_you ? "Ability_Slash_NoStamina_You"
				                     : "Ability_Slash_NoStamina_Opponent";
			}
		} else {
			return caster_is_you ? "Ability_Slash_Execute_You" : "Ability_Slash_Execute_Opponent";
		}

		assert("fail" && false);
	}
};

class DefendPlayerAbility : public PlayerAbility {
	public:
	static std::string id() { return "defend"; }
	std::string ability_id() const { return id(); }

	std::vector<std::string> commands() const { return {"defend"}; }
	std::string ability_description_key() const { return "Ability_Defend_Description"; }

	PlayerAbilityExecution execute(const Player& caster, const Player&) const {
		PlayerAbilityExecution effect;

		// check that the caster has enough stamina
		if (caster.stamina() < 2) {
			effect.failure = true;
			effect.failure_reason = PLAYERABILITYEXECUTIONFAILUREREASON_NOTENOUGHSTAMINA;
			return effect;
		}

		effect.effect_on_caster.effect_on_stamina(-2);
		effect.effect_on_caster.effect_on_defense(+4);

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
	static std::string id() { return "idle"; }
	std::string ability_id() const { return id(); }

	std::vector<std::string> commands() const { return {"nothing", "idle"}; }
	std::string ability_description_key() const { return "Ability_Idle_Description"; }

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
	static std::string id() { return "forfeit"; }
	std::string ability_id() const { return id(); }

	std::vector<std::string> commands() const { return {"forfeit", "quit"}; }
	std::string ability_description_key() const { return "Ability_Forfeit_Description"; }

	PlayerAbilityExecution execute(const Player&, const Player&) const {
		PlayerAbilityExecution effect;

		return effect;
	}

	std::string message(const PlayerAbilityExecution&, bool caster_is_you) const {
		return caster_is_you ? "Ability_Forfeit_Execute_You" : "Ability_Forfeit_Execute_Opponent";
	}
};
