#include "Player.h"
#include "ActionRequest.h"

Player::Player()
	: _name(L"unnamed")
{
}


Player::~Player()
{
}

void Player::quitRequest(ActionRequest& request) const
{
	request.action_type(ACTIONTYPE_FORFEIT);
}