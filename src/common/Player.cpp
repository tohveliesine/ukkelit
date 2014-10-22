#include "Player.h"
#include "ActionRequest.h"

Player::Player(const std::wstring& name)
	: _name(name)
{
}


Player::~Player()
{
}

void Player::quitRequest(ActionRequest& request) const
{
	request.action_type(ACTIONTYPE_FORFEIT);
}