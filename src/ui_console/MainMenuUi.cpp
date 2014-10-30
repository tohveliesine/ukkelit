#include <memory>
#include <cassert>
#include <iostream>
#include <map>

#include "UserInterface.h"

static enum Command {
	COMMAND_UNKNOWN,
	COMMAND_RANDOM_GAME,
	COMMAND_OPTIONS,
	COMMAND_HELP,
	COMMAND_QUIT,
};

static std::map<std::string, Command> commands = {
    {"random", COMMAND_RANDOM_GAME}, {"options", COMMAND_OPTIONS}, {"help", COMMAND_HELP}, {"quit", COMMAND_QUIT},
};

std::unique_ptr<UserInterface> MainMenuUi::run_ui() {
	co.write_line();
	co.write_line();
	co.write_line(t("MainMenu_QuickHelp"));

	// ask user what to do
	Command command = COMMAND_UNKNOWN;
	while (command == COMMAND_UNKNOWN) {
		std::string command_input(prompt("help"));

		// try to look for an exact match
		if (commands.count(command_input) == 0) {
			// not found
			continue;
		}

		command = commands[command_input];
	}

	// return ui based on the command
	if (command == COMMAND_RANDOM_GAME) {
		return std::unique_ptr<UserInterface>(new ConnectRandomGameUi());
	} else if (command == COMMAND_HELP) {
		return std::unique_ptr<UserInterface>(new HelpUi());
	}

	return nullptr;
}