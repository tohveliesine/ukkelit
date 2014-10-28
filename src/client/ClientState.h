#pragma once

#include <string>

#include "../common/Player.h"
#include "../common/ServerCommunication.h"
#include "../common/GameState.h"

class ClientGameState : public GameState {
	SessionId _session_id;

	public:
	const Player& self() const { return player_a(); }
	Player& self() { return player_a(); }

	const Player& opponent() const { return player_b(); }
	Player& opponent() { return player_b(); }

	bool is_self_turn() const { return &self() == &player_turn(); }

	SessionId session_id() const { return _session_id; }
	void session_id(SessionId session_id) { _session_id = session_id; }
};

class ClientState {
	std::string _alias_name;

	std::shared_ptr<ClientGameState> _gamestate;
	std::shared_ptr<ServerCommunication> _server_communication;

	public:
	const std::string& alias_name() const { return _alias_name; }
	std::string& alias_name() { return _alias_name; }
	void alias_name(const std::string& alias_name) { _alias_name = alias_name; }

	const ClientGameState* gamestate() const { return _gamestate.get(); }
	ClientGameState* gamestate() { return _gamestate.get(); }
	void gamestate(std::shared_ptr<ClientGameState> gamestate) { _gamestate = gamestate; }

	std::shared_ptr<ServerCommunication> server_communication() { return _server_communication; }
	void server_communication(std::shared_ptr<ServerCommunication> server_communication) {
		_server_communication = server_communication;
	}
};
