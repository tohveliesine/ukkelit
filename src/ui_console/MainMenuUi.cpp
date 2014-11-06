#include <memory>
#include <cassert>
#include <iostream>
#include <map>

#include "UserInterface.h"

enum class Command {
	UNKNOWN,
	RANDOM_GAME,
	OPTIONS,
	HELP,
	QUIT,
};

static std::map<std::string, Command> commands = {
    {"random", Command::RANDOM_GAME}, {"options", Command::OPTIONS}, {"help", Command::HELP}, {"quit", Command::QUIT},
};

std::unique_ptr<UserInterface> MainMenuUi::run_ui() {
	co.write_line();
	co.write_line();
	co.write_line(t("MainMenu_QuickHelp"));

	// ask user what to do
	Command command = Command::UNKNOWN;
	while (command == Command::UNKNOWN) {
		std::string command_input(prompt("help"));

		// try to look for an exact match
		if (commands.count(command_input) == 0) {
			// not found
			continue;
		}

		command = commands[command_input];
	}

	// return ui based on the command
	if (command == Command::RANDOM_GAME) {
		return std::unique_ptr<UserInterface>(new ConnectRandomGameUi());
	} else if (command == Command::HELP) {
		return std::unique_ptr<UserInterface>(new HelpUi());
	}

	return nullptr;
}
