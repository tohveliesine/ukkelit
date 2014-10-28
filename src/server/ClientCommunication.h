#pragma once

#include <memory>

#include "../common/ServerMessages.h"
#include "../common/ClientMessages.h"

class MockNetwork;

// used by server to handle communication to a single client
class ClientCommunication {
	MockNetwork* _mock_network;

      public:
	ClientCommunication(MockNetwork* mock_network);
	~ClientCommunication();

	// server calls to send a message to client
	void send_message(std::shared_ptr<const ServerMessage> message);
};
