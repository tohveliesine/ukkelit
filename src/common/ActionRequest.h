#pragma once

#include "ActionType.h"

class Player;

class ActionRequest
{
	ActionType _action_type;
	Player* _caster;
	Player* _target;

public:
	ActionType action_type() const { return _action_type; }
	void action_type(ActionType action_type) { _action_type = action_type; }

	const Player* caster() const { return _caster; }
	Player* caster() { return _caster; }
	void caster(Player* caster) { _caster = caster; }

	const Player* target() const { return _target; }
	Player* target() { return _target; }
	void target(Player* target) { _target = target; }

	ActionRequest();
	~ActionRequest();
};

