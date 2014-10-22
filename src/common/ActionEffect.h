#pragma once
class ActionEffect
{
	int _healthpoints;
	int _defense;
	int _stamina;

public:
	int healthpoints() const { return _healthpoints; }
	void healthpoints(int healthpoints) { _healthpoints = healthpoints; }

	int defense() const { return _defense; }
	void defense(int defense) { _defense = defense; }

	int stamina() const { return _stamina; }
	void stamina(int stamina) { _stamina = stamina; }

	ActionEffect();
	~ActionEffect();
};

