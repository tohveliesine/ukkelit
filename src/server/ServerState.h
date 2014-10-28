#pragma once

#include <memory>
#include "../common/GameState.h"

class ClientCommunication;

class ServerGameState : public GameState {
	SessionId _player_a_sessionid;
	SessionId _player_b_sessionid;

	PlayerId _winner;

	public:
	ServerGameState() : _winner(0), _player_a_sessionid(0), _player_b_sessionid(0) {}

	SessionId player_a_sessionid() const { return _player_a_sessionid; }
	void player_a_sessionid(SessionId sessionid) { _player_a_sessionid = sessionid; }

	SessionId player_b_sessionid() const { return _player_b_sessionid; }
	void player_b_sessionid(SessionId sessionid) { _player_b_sessionid = sessionid; }

	SessionId session_turn() const {
		if (&player_turn() == &player_a()) {
			return player_a_sessionid();
		} else {
			return player_b_sessionid();
		}
	}

	const PlayerId winner() const { return _winner; }
	void winner(PlayerId winner) { _winner = winner; }
};

class ServerState {
	std::shared_ptr<ClientCommunication> _client_communication;
	ServerGameState _gamestate;

	public:
	std::shared_ptr<ClientCommunication> client_communication() { return _client_communication; }
	void client_communication(std::shared_ptr<ClientCommunication> client_communication) {
		_client_communication = client_communication;
	}

	const ServerGameState& gamestate() const { return _gamestate; }
	ServerGameState& gamestate() { return _gamestate; }
};