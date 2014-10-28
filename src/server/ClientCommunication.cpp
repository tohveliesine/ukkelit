#include "ClientCommunication.h"
#include "../common/ServerCommunication.h"

ClientCommunication::ClientCommunication(MockNetwork* mock_network) : _mock_network(mock_network) {}

ClientCommunication::~ClientCommunication() {}

void ClientCommunication::send_message(std::shared_ptr<const ServerMessage> message) {
	_mock_network->messages_from_server().push(message);
}