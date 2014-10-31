#include <memory>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <boost/algorithm/string/join.hpp>

#include "UserInterface.h"
#include "../client/ClientState.h"
#include "../common/Ability.h"

std::unique_ptr<UserInterface> GameUi::run_ui() {
	assert(clientstate()->gamestate() != nullptr);
	assert(clientstate()->server_communication() != nullptr);

	// join the queue
	std::shared_ptr<JoinRandomGameQueueClientMessage> join_queue_message(new JoinRandomGameQueueClientMessage());
	join_queue_message->alias_name = clientstate()->alias_name();
	clientstate()->server_communication()->send_message(join_queue_message);

	while (true) {
		// handle incoming messages
		int count = clientstate()->server_communication()->receive_messages(*this);

		// there should always be messages!
		if (count == 0) {
			co.write_line(t("Game_Warning_NoMessages"));
			break;
		}
	}

	return std::unique_ptr<UserInterface>(new MainMenuUi());
}

void GameUi::visit(const JoinedRandomGameQueueMessage& message) {
	co.write_line(t("Game_Queued"));

	clientstate()->gamestate()->session_id(message.session_id);
	clientstate()->gamestate()->self().player_id(message.player_id);
}
void GameUi::visit(const RandomGameFoundMessage& message) {
	co.write_line(t("Game_Queue_GameFound"));

	// print out quick help
	co.write_line(t("Game_QuickGuide"));
}
void GameUi::visit(const GameStartedMessage& message) {
	co.write_line(t("Game_Starting"));

	const Player* self;
	const Player* opponent;
	if (clientstate()->gamestate()->self().player_id() == message.player_a.player_id()) {
		self = &message.player_a;
		opponent = &message.player_b;
	} else {
		self = &message.player_b;
		opponent = &message.player_a;
	}

	clientstate()->gamestate()->self() = *self;
	clientstate()->gamestate()->opponent() = *opponent;
}
void GameUi::visit(const GameEndedMessage& message) {
	Player& winner(clientstate()->gamestate()->player_a().player_id() == message.winner_player_id
	                   ? clientstate()->gamestate()->player_a()
	                   : clientstate()->gamestate()->player_b());

	co.write_line(t("Game_Ended", winner.name()));
}

static enum ActionChoice {
	GAMEUI_ACTIONCHOICE_ATTACK,
	GAMEUI_ACTIONCHOICE_DEFEND,
	GAMEUI_ACTIONCHOICE_NOTHING,
	GAMEUI_ACTIONCHOICE_FORFEIT,
};

static std::map<std::string, ActionChoice> ActionChoiceMap = {
    {"attack", GAMEUI_ACTIONCHOICE_ATTACK},
    {"defend", GAMEUI_ACTIONCHOICE_DEFEND},
    {"nothing", GAMEUI_ACTIONCHOICE_NOTHING},
    {"forfeit", GAMEUI_ACTIONCHOICE_FORFEIT},
    {"quit", GAMEUI_ACTIONCHOICE_FORFEIT},
};

static ActionChoice get_action_choice(ConsoleOutput& co, std::istream& in) {
	co.write_line(t("Game_Choices"));

	bool first_time = true;
	while (true) {
		co.write(t("Game_Prompt"));

		std::string command;
		std::getline(in, command);

		if (ActionChoiceMap.count(command) == 1) {
			return ActionChoiceMap[command];
		}

		if (first_time) {
			co.write_line(t("Game_ChoicesHelp"));

			first_time = false;
		}
	}
}

void GameUi::action() {
	co.write_line(t("Game_TurnHint_You"));

	ActionChoice choice = get_action_choice(co, *in());

	std::shared_ptr<ClientMessage> client_message;
	switch (choice) {
	case GAMEUI_ACTIONCHOICE_ATTACK:
		client_message =
		    clientstate()->gamestate()->self().attack_request(clientstate()->gamestate()->session_id());

		break;
	case GAMEUI_ACTIONCHOICE_DEFEND:
		client_message =
		    clientstate()->gamestate()->self().defend_request(clientstate()->gamestate()->session_id());

		break;
	case GAMEUI_ACTIONCHOICE_NOTHING:
		client_message =
		    clientstate()->gamestate()->self().idle_request(clientstate()->gamestate()->session_id());

		break;
	case GAMEUI_ACTIONCHOICE_FORFEIT:
		client_message =
		    clientstate()->gamestate()->self().forfeit_request(clientstate()->gamestate()->session_id());

		break;
	}

	if (client_message != nullptr) {
		clientstate()->server_communication()->send_message(client_message);
	}
}

void GameUi::visit(const TurnChangedMessage& message) {
	clientstate()->gamestate()->player_turn(message.player_id);
	clientstate()->gamestate()->turn_counter(message.turn_number);
	clientstate()->gamestate()->player_turn().apply(message.effect_on_player);

	int sta_gained = message.effect_on_player.effect_on_stamina();
	if (sta_gained > 0) {
	if (clientstate()->gamestate()->is_self_turn()) {
		co.write_line(t("Game_StaminaRegeneration_You", sta_gained));
	} else {
			co.write_line(t("Game_StaminaRegeneration_Opponent",
			                clientstate()->gamestate()->opponent().name(), sta_gained));
		}
	}

	print_status();

	if (clientstate()->gamestate()->is_self_turn()) {
		action();
	} else {
		co.write_line(t("Game_TurnHint_Opponent", clientstate()->gamestate()->opponent().name()));
	}
}

class SummaryFormatter {
	const Player& _self;
	const Player& _opponent;

	public:
	SummaryFormatter(const Player& self, const Player& opponent) : _self(self), _opponent(opponent) {}

	std::string format_action(const PlayerActionMessage& message) {
		std::ostringstream sb;

		auto ability = PlayerAbility::get_ability_by_id(message.action_request.ability_id);
		assert(ability != nullptr);

		bool is_caster_self = (message.caster_player_id == _self.player_id());
		const Player& caster = is_caster_self ? _self : _opponent;
		const Player& target = is_caster_self ? _opponent : _self;

		return t(ability->message(message.execution, is_caster_self), caster.name(), target.name(),
		         message.execution.effect_on_target.total_damage());
	}

	std::string format_effects(const PlayerActionMessage& message) {
		// You lose -6 hitpoints.
		// Opponent loses -4 stamina.

		// Opponent loses -6 hitpoints.
		// You lose -4 stamina.

		// You gain +4 defense, lose -2 stamina.

		// Opponent gains +4 defense, loses -2 stamina.

		std::ostringstream sb;
		bool is_caster_self = (message.caster_player_id == _self.player_id());
		if (!message.execution.effect_on_caster.empty()) {
			sb << stat_player(message.execution.effect_on_caster, is_caster_self) << std::endl;
		}
		if (!message.execution.effect_on_target.empty()) {
			sb << stat_player(message.execution.effect_on_target, !is_caster_self) << std::endl;
		}

		return sb.str();
	}

	static std::string stat_player(const ActionEffect& effect, bool is_self) {
		std::string pronoun(is_self ? "You" : "Opponent");
		std::string gains(is_self ? "gain" : "gains");
		std::string loses(is_self ? "lose" : "loses");

		std::ostringstream sb;
		sb << pronoun << " ";

		std::vector<std::string> effects;
		if (effect.effect_on_healthpoints()) {
			effects.push_back(stat_message(effect.effect_on_healthpoints(), gains, loses) +
			                  " healthpoints");
		}
		if (effect.effect_on_defense()) {
			effects.push_back(stat_message(effect.effect_on_defense(), gains, loses) + " defense");
		}
		if (effect.effect_on_stamina()) {
			effects.push_back(stat_message(effect.effect_on_stamina(), gains, loses) + " stamina");
		}

		sb << boost::algorithm::join(effects, ", ");

		return sb.str();
	}

	static std::string stat_message(ActionEffect::Points value, const std::string& gains,
	                                const std::string& loses) {
		assert(value != 0);

		std::ostringstream sb;

		if (value > 0) {
			sb << gains << " +" << value;
		} else {
			sb << loses << " " << value;
		}

		return sb.str();
	}
};

void GameUi::visit(const PlayerActionMessage& message) {
	Player& caster = clientstate()->gamestate()->player_turn();
	Player& target = clientstate()->gamestate()->player_waiting();
	caster.apply(message.execution.effect_on_caster);
	target.apply(message.execution.effect_on_target);

	bool is_caster_self = (caster.player_id() == clientstate()->gamestate()->self().player_id());
	Player& self = is_caster_self ? caster : target;
	Player& opponent = is_caster_self ? target : caster;

	ConsoleOutput action_co(co.marginh_custom_left_str(">> "));
	SummaryFormatter formatter(self, opponent);
	co.write_line();
	action_co.write_line(formatter.format_action(message));
	co.write_line();

	std::string effects(formatter.format_effects(message));
	if (!effects.empty()) {
		co.write_line(effects);
		co.write_line();
	}
}

std::string player_stats(const Player& player) {
	// (20 hp, 0 def, 10 sta)

	return t("Game_Stats_Display", player.healthpoints(), player.defense(), player.stamina());
}

void GameUi::print_status() {
	// output is 72 chars wide with 4 space margins on the sides
	// 0         1         2         3         4         5         6         7
	// 0123456789012345678901234567890123456789012345678901234567890123456789012
	//                                   [0]
	//     tohveli (you)                  vs         > eräjorma < (opponent)
	//     (20 hp, 0 def, 10 sta)                     (20 hp, 0 def, 10 sta)

	// extra newlines between turns
	co.write_line();
	co.write_line();

	// turn counter
	co.write_center(t("Game_Turn_Counter", clientstate()->gamestate()->turn_counter()));

	// player names
	co.write_fill(t("Game_Turn_Players_You", clientstate()->gamestate()->self().name()), t("Game_Turn_Players_vs"),
	              t("Game_Turn_Players_Opponent", clientstate()->gamestate()->opponent().name()));

	// player stats
	co.write_fill(player_stats(clientstate()->gamestate()->self()), "",
	              player_stats(clientstate()->gamestate()->opponent()));

	// extra newline at end
	co.write_line();
}