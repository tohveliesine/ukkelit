#include <iostream>
#include <string>

#include "Main.h"
#include "UserInterface.h"

Main::Main() {}

Main::~Main() {}

void Main::run() {
	StartupUi startupUi;
	std::shared_ptr<ClientState> clientstate(new ClientState());
	std::unique_ptr<UserInterface> ui = startupUi.run(clientstate, &std::cin);
	while (ui != nullptr) {
		std::cout << std::endl;

		ui = ui->run(clientstate, &std::cin);
	}

	std::cout << "Press RETURN to quit." << std::endl;
	std::string s;
	std::getline(std::cin, s);
}