#include <string>
#include <map>

#include "Translations.h"

// clang-format off
static std::map<std::string, std::string> translations = {
    {"Startup_Name", "Ukkelit v{0}"}, // {0} is the version number, e.g. 0.1
    {"Startup_Intro", "A 1 vs 1 strategic hero combat game."},
    {"FirstTime_AliasQuestion", "Since this is your first time playing, state your alias, please:"},
    {"FirstTime_Error_NameTooLong", "Please use at most {0} characters in your name."}, // {0} is the max length
    {"FirstTime_Welcome", "Welcome to the game, {0}! You may find it easiest to learn\r\nthe game by simply joining a "
                          "random game."}, // {0} is the player's name
    {"ConnectRandomGame_Connecting", "Connecting... OK!"},
    {"MainMenu_QuickHelp", "Do you want to find a /random/ game, /help/ or /quit/?"},
    {"Game_Warning_NoMessages", "Warning: No more messages to process."},
    {"Game_Queued", "You are in the queue for a random game."},
    {"Game_Queue_GameFound", "Found someone to play with!"},
    {"Game_Starting", "Game starting..."},
    {"Game_Ended", "    >> Game ended. Winner is {0} <<"}, // {0} is the player name
    {"Game_Choices", "    Your choices are to /attack/, /defend/ or do /nothing/."},
    {"Game_Prompt", "    ?> "},
    {"Game_ChoicesHelp", "    /attack/ costs -4 sta, but deals +5 damage.\r\n"
                         "    /defend/ costs -2 sta, but gives +3 defense.\r\n"
                         "    do /nothing/ if you have not enough stamina and wish to regenerate\r\n"
                         "    You may also /forfeit/ if you feel like a loser."},
    {"Game_TurnHint_You", "    It is your turn. What will you do?"},
    {"Game_TurnHint_Opponent", "    It is now {0}'s turn.\r\n    Wait for her action..."}, // {0} is player's name
    {"Game_StaminaRegeneration_You", "    Your stamina regenerates, and you gain {0:+d} stamina."}, // {0} is stamina gained
    {"Game_StaminaRegeneration_Opponent", "    {0}'s stamina regenerates, and she gains {1:+d} stamina."}, // {0} is player's name, {1} is stamina gained
	{"Game_WaitUntilRegeneration_You", "    You will have to wait until it regenerates next turn." },
	{"Game_WaitUntilRegeneration_Opponent", "    {0} will have to wait until it regenerates next turn." }, // {0} is player's name
	{"Game_Stats_Display", "({0} hp, {1} def, {2} sta)"}, // {0} is healthpoints value, {1} is defense value, {2} is stamina value
	{"Ability_Attack_NoStamina_You", "    You want to attack, but fail to note that you have no stamina left."},
    {"Ability_Attack_NoStamina_Opponent", "    {0} wants to attack, but fails to note that she has no stamina left."}, // {0} is player's name
    {"Ability_Defend_NoStamina_You", "    You want to take a defensive position, but fail to note that you have no stamina left."},
    {"Ability_Defend_NoStamina_Opponent", "    {0} wants to take a defensive position, but fails to note that she has no stamina left."}, // {0} is player's name
    {"Ability_Attack_Execute_You", "You strike {0} with a sword,\r\n       dealing {1} damage across her body."}, // {0} is target's name, {1} damage dealt
    {"Ability_Attack_Execute_Opponent", "{0} strikes you with a sword,\r\n       dealing {1} damage across your body."}, // {0} is caster's name, {1} is damage dealt
    {"Ability_Defend_Execute_You", "You get in a defensive position."},
    {"Ability_Defend_Execute_Opponent", "{0} gets in a defensive position."}, // {0} is player's name
    {"Ability_Generic_Execute_You", "You do something incomprehensible."},
    {"Ability_Generic_Execute_Opponent", "{0} does something incomprehensible."}, // {0} is player's name
    {"Ability_Forfeit_Execute_You", "You decide to give up, like a coward."},
    {"Ability_Forfeit_Execute_Opponent", "{0} decides to give up, like a coward."}, // {0} is player's name
    {"Ability_Idle_Execute_You", "You are unsure of your ability to do anything meaningful,\r\n       and opt to do nothing."},
    {"Ability_Idle_Execute_Opponent", "{0} is unsure of her ability to do anything meaningful,\r\n       and opts to do nothing."}, // {0} is player's name
	{"Help_Main", "Ukkelit is a networked game for fast one versus one combat matches.\r\n"
                  "Mastering it requires strategy and practice.\r\n"
                  "\r\n"
                  "Your alias will not be reserved, and there is no progress to be saved\r\n"
                  "anywhere, and thus all players are on the same line regardless of the\r\n"
                  "time played.\r\n"
                  "\r\n"
                  "Try typing one of these commands to get started:\r\n"
                  "    random     - connect to the Internet to automatically find\r\n"
                  "                 someone to play a game with.\r\n"
                  "    alias      - change your alias.\r\n"
                  "    proxy      - show or change your http proxy settings.\r\n"
                  "    help       - print this help.\r\n"
                  "    help game - print basic help and tips about the gameplay.\r\n"
                  "    quit       - exit to console.\r\n"
                  "\r\n"
                  "Partial commands are also recognized, e.g. /q/ for /quit/." },
    {"Game_QuickGuide", "Quick guide - While you wait for the game to start\r\n"
                        "\r\n"
                        "                    hitpoints        stamina\r\n"
                        "      Deprive the opponent   \\       /   Reduced by actions,\r\n"
                        "	      to win the game.  (20, 0, 10)  regenerates by +3 every turn.\r\n"
                        "                                 /\r\n"
                        "                              defense\r\n"
                        "                         Shields you from attacks,\r\n"
                        "                         protecting your hitpoints." },
};
// clang-format on

Translations::Translations() {}

Translations::~Translations() {}

std::string Translations::get(const std::string& key) const {
	if (translations.count(key) == 0) {
		return "[*" + key + "]";
	}

	return translations[key];
}