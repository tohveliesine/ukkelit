#pragma once

#include <string>

#include "ActionEffect.h"

enum AbilityType {
	ABILITYTYPE_MELEE,
	ABILITYTYPE_DEFENSE,
};

class PlayerAbility {
	public:
	virtual std::string name() = 0;
	virtual AbilityType ability_type() = 0;
	virtual const ActionEffect& effect_on_caster() = 0;
	virtual const ActionEffect& effect_on_target() = 0;
};

class AttackPlayerAbility : public PlayerAbility {
	static struct static_data {
		ActionEffect _effect_on_caster;
		ActionEffect _effect_on_target;

		static_data() {
			_effect_on_caster.effect_on_stamina(-4);

			_effect_on_target.effect_on_healthpoints(-6);
		}
	} _data;

	public:
	std::string name() { return "attack"; }
	AbilityType ability_type() { return ABILITYTYPE_MELEE; }
	const ActionEffect& effect_on_caster() { return _data._effect_on_caster; }
	const ActionEffect& effect_on_target() { return _data._effect_on_target; }
};

class DefensePlayerAbility : public PlayerAbility {
	private:
	static struct static_data {
		ActionEffect _effect_on_caster;
		ActionEffect _effect_on_target;

		static_data() {
			_effect_on_caster.effect_on_stamina(-2);
			_effect_on_caster.effect_on_defense(+4);
		}
	} _data;

	public:
	std::string name() { return "defense"; }
	AbilityType ability_type() { return ABILITYTYPE_DEFENSE; }
	const ActionEffect& effect_on_caster() { return _data._effect_on_caster; }
	const ActionEffect& effect_on_target() { return _data._effect_on_target; }
};