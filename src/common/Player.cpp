#include "Player.h"
#include "ClientMessages.h"
#include "ActionEffect.h"

Player::Player() : _name("unnamed"), _healthpoints(20), _defense(0), _stamina(10) {}

Player::~Player() {}

std::unique_ptr<ClientMessage> Player::forfeit_request(SessionId session_id) const {
	auto request = new PlayerActionRequestClientMessage();
	std::unique_ptr<ClientMessage> message(request);
	request->action_type = PLAYERACTIONTYPE_FORFEIT;
	request->session_id = session_id;

	return message;
}

std::unique_ptr<ClientMessage> Player::attack_request(SessionId session_id) const {
	auto request = new PlayerActionRequestClientMessage(PLAYERABILITYTYPE_ATTACK);
	std::unique_ptr<ClientMessage> message(request);
	request->action_type = PLAYERACTIONTYPE_ABILITY;
	request->session_id = session_id;

	return message;
}

std::unique_ptr<ClientMessage> Player::defend_request(SessionId session_id) const {
	auto request = new PlayerActionRequestClientMessage(PLAYERABILITYTYPE_DEFEND);
	std::unique_ptr<ClientMessage> message(request);
	request->action_type = PLAYERACTIONTYPE_ABILITY;
	request->session_id = session_id;

	return message;
}

std::unique_ptr<ClientMessage> Player::idle_request(SessionId session_id) const {
	auto request = new PlayerActionRequestClientMessage();
	std::unique_ptr<ClientMessage> message(request);
	request->action_type = PLAYERACTIONTYPE_IDLE;
	request->session_id = session_id;

	return message;
}

void Player::apply(const ActionEffect& effect) {
	_healthpoints += effect.effect_on_healthpoints();
	_defense += effect.effect_on_defense();
	_stamina += effect.effect_on_stamina();
}