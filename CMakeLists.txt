cmake_minimum_required (VERSION 2.6)
project (Ukkelit)

add_definitions (
	-std=c++11
	-W -Wall -Werror
)

include_directories (
	cppformat/
)

add_executable(ukkelit 
	src/common/Ability.cpp
	src/common/ActionEffect.cpp
	src/common/Player.cpp
	src/common/ServerCommunication.cpp
	src/common/Translations.cpp
	src/game/GameLogic.cpp
	src/server/ClientCommunication.cpp
	src/ui_console/ConnectRandomGameUi.cpp
	src/ui_console/ConsoleOutput.cpp
	src/ui_console/entry.cpp
	src/ui_console/FirstTimeUi.cpp
	src/ui_console/GameUi.cpp
	src/ui_console/HelpUi.cpp
	src/ui_console/Main.cpp
	src/ui_console/MainMenuUi.cpp
	src/ui_console/StartupUi.cpp)
target_link_libraries(
	ukkelit
	${CMAKE_SOURCE_DIR}/cppformat/libformat.a)
