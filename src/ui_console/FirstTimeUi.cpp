#include <memory>
#include <cassert>
#include <string>

#include "UserInterface.h"

std::unique_ptr<UserInterface> FirstTimeUi::run_ui() {
	assert(out() != nullptr);
	assert(in() != nullptr);

	std::cout << std::endl;
	std::cout << std::endl;
	*out() << "Since this is your first time playing, state your alias, please:" << std::endl;

	while (clientstate()->alias_name().empty()) {
		std::string name(prompt("anonymous coward"));

		if (name.length() > 16) {
			*out() << "Please use at most 16 characters in your name." << std::endl;
		} else {
			clientstate()->alias_name(name);
		}
	}

	*out() << std::endl;
	*out() << "Welcome to the game, " << clientstate()->alias_name() << "! You may find it easiest to learn"
	       << std::endl << "the game by simply joining a random game.";

	return std::unique_ptr<UserInterface>(new MainMenuUi());
}
