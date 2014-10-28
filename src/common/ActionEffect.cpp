#include "ActionEffect.h"
#include "Player.h"

ActionEffect::ActionEffect()
    : _effect_on_healthpoints(0), _effect_on_defense(0), _effect_on_stamina(0), _total_damage(0) {}

ActionEffect::~ActionEffect() {}

void ActionEffect::deal_damage(int damage, const Player& target) {
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
	} else if (effectiveDefense == 0) {
		// case 4
		effect_on_healthpoints(-damage);
	} else {
		// case 3
		int damageLeft = damage - effectiveDefense;
		effect_on_defense(-effectiveDefense);
		effect_on_healthpoints(-damageLeft);
	}
}