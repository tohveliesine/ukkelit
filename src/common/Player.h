#pragma once

#include <memory>
#include <string>

#include "../common/CommonTypes.h"

struct ClientMessage;

class ActionEffect;

class Player {
	PlayerId _player_id;

	std::string _name;

	int _healthpoints;
	int _defense;
	int _stamina;

	public:
	PlayerId player_id() const { return _player_id; }
	void player_id(PlayerId player_id) { _player_id = player_id; }

	const std::string& name() const { return _name; }
	void name(const std::string& name) { _name = name; }

	int healthpoints() const { return _healthpoints; }
	void healthpoints(int healthpoints) { _healthpoints = healthpoints; }

	int defense() const { return _defense; }
	void defense(int defense) { _defense = defense; }

	int stamina() const { return _stamina; }
	void stamina(int stamina) { _stamina = stamina; }

	bool is_alive() const { return _healthpoints > 0; }

	Player();
	~Player();

	std::unique_ptr<ClientMessage> attack_request(SessionId session_id) const;
	std::unique_ptr<ClientMessage> defend_request(SessionId session_id) const;
	std::unique_ptr<ClientMessage> idle_request(SessionId session_id) const;
	std::unique_ptr<ClientMessage> forfeit_request(SessionId session_id) const;
	void apply(const ActionEffect& effect);
};