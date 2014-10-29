#include <memory>
#include <cassert>
#include <iostream>

#include "UserInterface.h"

std::unique_ptr<UserInterface> ConnectRandomGameUi::run_ui() {
	assert(out() != nullptr);

	// set up mock server and client communications
	std::shared_ptr<MockNetwork> mock_network(new MockNetwork());
	auto server_communication = std::shared_ptr<ServerCommunication>(new ServerCommunication(mock_network));
	clientstate()->server_communication(server_communication);

	// set up game state
	auto gamestate = std::shared_ptr<ClientGameState>(new ClientGameState());
	clientstate()->gamestate(gamestate);
	gamestate->self().name(clientstate()->alias_name());

	*out() << t("ConnectRandomGame_Connecting") << std::endl;

	return std::unique_ptr<UserInterface>(new GameUi());
}