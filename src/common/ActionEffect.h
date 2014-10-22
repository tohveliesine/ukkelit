#pragma once
#include <cmath>
#include <cassert>

#include "Hero.h"

class ActionEffect
{
	int _total_damage;
	int _effect_on_healthpoints;
	int _effect_on_defense;
	int _effect_on_stamina;

public:
	int effect_on_healthpoints() const { return _effect_on_healthpoints; }
	void effect_on_healthpoints(int healthpoints) { _effect_on_healthpoints += healthpoints; }

	int effect_on_defense() const { return _effect_on_defense; }
	void effect_on_defense(int defense) { _effect_on_defense += defense; }

	int effect_on_stamina() const { return _effect_on_stamina; }
	void effect_on_stamina(int stamina) { _effect_on_stamina += stamina; }

	int total_damage() const { return _total_damage; }

	void deal_damage(int damage, const Hero& target) {
		assert(damage >= 0);

		_total_damage += damage;

		// Case 1: Target has more than enough defense to absorb the damage
		//   Damage   Target             Effect
		//     6      hp 20, def 10      def -6, hp 0
		// Case 2: Target has just enough defense to absorb the damage
		//   Damage   Target             Effect
		//     6      hp 20, def 6       def -6, hp 0
		// Case 3: Target does not have enough defense to absorb the damage
		//   Damage   Target             Effect
		//     6      hp 20, def 5       def -5, hp -1
		// Case 4: Target does has 0 defense, cannot absorb any damage
		//   Damage   Target             Effect
		//     6      hp 20, def 0       def 0, hp -6

		int effectiveDefense = target.defense() - effect_on_defense();
		if (effectiveDefense >= damage) {
			// case 1 & 2
			effect_on_defense(-damage);
		}
		else if (effectiveDefense == 0) {
			// case 4
			effect_on_healthpoints(-damage);
		}
		else {
			// case 3
			int damageLeft = damage - effectiveDefense;
			effect_on_defense(-effectiveDefense);
			effect_on_healthpoints(-damageLeft);
		}
	}

	ActionEffect();
	~ActionEffect();
};

