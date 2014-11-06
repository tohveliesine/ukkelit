#include "Player.h"
#include "ClientMessages.h"
#include "ActionEffect.h"

Player::Player()
    : _available_abilities({"slash", "defend", "idle", "forfeit"}), _name("unnamed"), _healthpoints(20), _defense(0),
      _stamina(10) {}

Player::~Player() {}

void Player::apply(const ActionEffect& effect) {
	_healthpoints += effect.effect_on_healthpoints();
	_defense += effect.effect_on_defense();
	_stamina += effect.effect_on_stamina();
}
