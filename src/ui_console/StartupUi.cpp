#include <memory>
#include <cassert>
#include <iostream>

#include "UserInterface.h"
#include "../common/_ver.h"

std::unique_ptr<UserInterface> StartupUi::run_ui() {
	assert(out() != nullptr);

	*out() << "Ukkelit v" << UKKELIT_VERSION << std::endl;
	*out() << std::endl;
	*out() << "A 1 vs 1 strategic hero combat game." << std::endl;

	return std::unique_ptr<UserInterface>(new FirstTimeUi());
}