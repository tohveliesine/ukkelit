#include <memory>
#include <cassert>
#include <string>

#include "UserInterface.h"

std::unique_ptr<UserInterface> FirstTimeUi::run_ui() {
	assert(out() != nullptr);
	assert(in() != nullptr);

	std::cout << std::endl;
	std::cout << std::endl;
	*out() << t("FirstTime_AliasQuestion") << std::endl;

	while (clientstate()->alias_name().empty()) {
		std::string name(prompt("anonymous coward"));

		if (name.length() > 16) {
			*out() << t("FirstTime_Error_NameTooLong", 16) << std::endl;
		} else {
			clientstate()->alias_name(name);
		}
	}

	*out() << std::endl;
	*out() << t("FirstTime_Welcome", clientstate()->alias_name());

	return std::unique_ptr<UserInterface>(new MainMenuUi());
}
