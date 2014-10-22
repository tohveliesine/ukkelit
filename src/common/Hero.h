#pragma once

#include "ActionRequest.h"

class ActionEffect;

class Hero
{
	int _healthpoints;
	int _defense;
	int _stamina;

public:
	int healthpoints() const { return _healthpoints; }
	int defense() const { return _defense; }
	int stamina() const { return _stamina; }

	bool is_alive() const { return _healthpoints > 0; }

	Hero();
	~Hero();

	void attackRequest(ActionRequest& request) const;
	void defendRequest(ActionRequest& request) const;

	void apply(const ActionEffect& effect);
};

