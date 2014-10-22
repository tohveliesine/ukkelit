#pragma once

#include <string>
#include "ActionRequest.h"
#include "Hero.h"

class Player
{
	std::wstring _name;
	Hero _hero;

public:
	const std::wstring& name() const { return _name; }

	const Hero& hero() const { return _hero; }
	Hero& hero() { return _hero; }

	Player(const std::wstring& name);
	~Player();

	void quitRequest(ActionRequest& request) const;
};