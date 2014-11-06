#include <chrono>
#include <thread>

#include "ServerCommunication.h"

ServerCommunication::ServerCommunication(std::shared_ptr<MockNetwork> mock_network) : _mock_network(mock_network) {}

ServerCommunication::~ServerCommunication() {}

void ServerCommunication::send_message(std::shared_ptr<const ClientMessage> message) {
	_mock_network->send_message_to_server(message);
}

int ServerCommunication::receive_messages(ServerCommunicationVisitor& visitor) {
	auto& received_messages = _mock_network->messages_from_server();

	int count = 0;
	while (!received_messages.empty()) {
		auto message = received_messages.front();
		received_messages.pop();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		message->accept(visitor);
		++count;
	}

	return count;
}