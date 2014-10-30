#include <memory>
#include <cassert>
#include <iostream>

#include "UserInterface.h"
#include "../common/_ver.h"

std::unique_ptr<UserInterface> StartupUi::run_ui() {
	co.write_line(t("Startup_Name", UKKELIT_VERSION));
	co.write_line();
	co.write_line(t("Startup_Intro"));

	return std::unique_ptr<UserInterface>(new FirstTimeUi());
}