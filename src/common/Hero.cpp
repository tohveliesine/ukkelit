#include "Hero.h"


Hero::Hero()
	: _healthpoints(20), _defense(0), _stamina(10)
{
}


Hero::~Hero()
{
}

void Hero::attackRequest(ActionRequest& request) const {
	request.action_type(ACTIONTYPE_ATTACK);
}

void Hero::defendRequest(ActionRequest& request) const {
	request.action_type(ACTIONTYPE_DEFEND);
}

void Hero::apply(const ActionEffect& effect) {
	_healthpoints += effect.healthpoints();
	_defense += effect.defense();
	_stamina += effect.stamina();
}