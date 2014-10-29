#include <memory>
#include <cassert>
#include <iostream>

#include "UserInterface.h"

std::unique_ptr<UserInterface> HelpUi::run_ui() {
	assert(out() != nullptr);

	*out() << t("Help_Main") << std::endl;

	return std::unique_ptr<UserInterface>(new MainMenuUi());
}