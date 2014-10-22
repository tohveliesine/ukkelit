#pragma once

#include <string>
#include "Hero.h"

class ActionRequest;

class Player
{
	std::wstring _name;
	Hero _hero;

public:
	const std::wstring& name() const { return _name; }
	void name(const std::wstring& name) { _name = name; }

	const Hero& hero() const { return _hero; }
	Hero& hero() { return _hero; }

	Player();
	~Player();

	void quitRequest(ActionRequest& request) const;
};