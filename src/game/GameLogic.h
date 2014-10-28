#pragma once

#include "../server/ClientCommunication.h"
#include "../server/ServerState.h"

class GameLogic : public ClientCommunicationVisitor {
	ServerState _server_state;

	void announce_next_turn();
	void announce_turn();
	void announce_game_started();
	void announce_game_ended();

	public:
	const ServerState& server_state() const { return _server_state; }
	ServerState& server_state() { return _server_state; }

	GameLogic();
	~GameLogic();

	void visit(const JoinRandomGameQueueClientMessage& message);
	void visit(const PlayerActionRequestClientMessage& message);
};
