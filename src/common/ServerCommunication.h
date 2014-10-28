#pragma once

#include <queue>
#include <memory>

#include "ServerMessages.h"
#include "ClientMessages.h"
#include "MockNetwork.h"

// used by clients to handle communication traffic from/to game server.
class ServerCommunication {
	std::shared_ptr<MockNetwork> _mock_network;

      public:
	ServerCommunication(std::shared_ptr<MockNetwork> mock_network);
	~ServerCommunication();

	// client calls to send a message to server
	void send_message(std::shared_ptr<const ClientMessage> message);

	// client calls to handle received messages from the server
	int receive_messages(ServerCommunicationVisitor& visitor);
};