#include <memory>
#include <cassert>
#include <string>

#include "UserInterface.h"

std::unique_ptr<UserInterface> FirstTimeUi::run_ui() {
	assert(in() != nullptr);

	co.write_line();
	co.write_line();
	co.write_line(t("FirstTime_AliasQuestion"));

	while (clientstate()->alias_name().empty()) {
		std::string name(prompt("anonymous coward"));

		if (name.length() > 16) {
			co.write_line(t("FirstTime_Error_NameTooLong", 16));
		} else {
			clientstate()->alias_name(name);
		}
	}

	co.write_line();
	co.write_line(t("FirstTime_Welcome", clientstate()->alias_name()));

	return std::unique_ptr<UserInterface>(new MainMenuUi());
}
