#pragma once
#include <cmath>
#include <cassert>

class Player;


class ActionEffect {
	public:
	typedef int Points;

	private:
	Points _total_damage;
	Points _effect_on_healthpoints;
	Points _effect_on_defense;
	Points _effect_on_stamina;

	public:
	Points effect_on_healthpoints() const { return _effect_on_healthpoints; }
	void effect_on_healthpoints(Points healthpoints) { _effect_on_healthpoints += healthpoints; }

	Points effect_on_defense() const { return _effect_on_defense; }
	void effect_on_defense(Points defense) { _effect_on_defense += defense; }

	Points effect_on_stamina() const { return _effect_on_stamina; }
	void effect_on_stamina(Points stamina) { _effect_on_stamina += stamina; }

	Points total_damage() const { return _total_damage; }

	bool empty() const {
		return effect_on_healthpoints() == 0
			&& effect_on_defense() == 0
			&& effect_on_stamina() == 0;
	}

	void deal_damage(Points damage, const Player& target);

	ActionEffect();
	~ActionEffect();
};