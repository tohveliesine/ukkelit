#include <memory>
#include <cassert>
#include <iostream>

#include "UserInterface.h"

std::unique_ptr<UserInterface> HelpUi::run_ui() {
	assert(out() != nullptr);

	*out() << "\
Ukkelit is a networked game for fast one versus one combat matches.\r\n\
Mastering it requires strategy and practice.\r\n\
\r\n\
Your alias will not be reserved, and there is no progress to be saved\r\n\
anywhere, and thus all players are on the same line regardless of the\r\n\
time played.\r\n\
\r\n\
Try typing one of these commands to get started:\r\n\
    random     - connect to the Internet to automatically find\r\n\
                 someone to play a game with.\r\n\
    alias      - change your alias.\r\n\
    proxy      - show or change your http proxy settings.\r\n\
    help       - print this help.\r\n\
    help game  - print basic help and tips about the gameplay.\r\n\
    quit       - exit to console.\r\n\
\r\n\
Partial commands are also recognized, e.g. /q/ for /quit/.\
";

	return std::unique_ptr<UserInterface>(new MainMenuUi());
}