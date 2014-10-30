#include <memory>
#include <cassert>
#include <iostream>

#include "UserInterface.h"

std::unique_ptr<UserInterface> HelpUi::run_ui() {
	co.write_line(t("Help_Main"));

	return std::unique_ptr<UserInterface>(new MainMenuUi());
}