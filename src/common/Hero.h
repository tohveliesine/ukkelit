#pragma once

#include "ActionRequest.h"
#include "ActionEffect.h"

class Hero
{
	int _healthpoints;
	int _defense;
	int _stamina;

public:
	int healthpoints() const { return _healthpoints; }
	int defense() const { return _defense; }
	int stamina() const { return _stamina; }

	Hero();
	~Hero();

	void attackRequest(ActionRequest& request) const;
	void defendRequest(ActionRequest& request) const;

	void apply(const ActionEffect& effect);
};

