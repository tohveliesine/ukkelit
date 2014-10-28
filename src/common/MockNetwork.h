#pragma once

#include <queue>

#include "ClientMessages.h"
#include "ServerMessages.h"
#include "../game/GameLogic.h"

class MockNetwork {
	GameLogic _gamelogic;
	std::queue<std::shared_ptr<const ServerMessage>> _messages_from_server;

	public:
	MockNetwork() {
		std::shared_ptr<ClientCommunication> client_communication(new ClientCommunication(this));
		_gamelogic.server_state().client_communication(client_communication);
	}

	void send_message_to_server(std::shared_ptr<const ClientMessage> message) { message->accept(_gamelogic); }

	std::queue<std::shared_ptr<const ServerMessage>>& messages_from_server() { return _messages_from_server; }
};
