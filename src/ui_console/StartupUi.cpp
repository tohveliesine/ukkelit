#include <memory>
#include <cassert>
#include <iostream>

#include "UserInterface.h"
#include "../common/_ver.h"

std::unique_ptr<UserInterface> StartupUi::run_ui() {
	assert(out() != nullptr);

	*out() << t("Startup_Name", UKKELIT_VERSION) << std::endl;
	*out() << std::endl;
	*out() << t("Startup_Intro") << std::endl;

	return std::unique_ptr<UserInterface>(new FirstTimeUi());
}