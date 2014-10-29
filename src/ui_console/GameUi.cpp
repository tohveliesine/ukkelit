#include <memory>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <boost/algorithm/string/join.hpp>

#include "UserInterface.h"
#include "../client/ClientState.h"

std::unique_ptr<UserInterface> GameUi::run_ui() {
	assert(out() != nullptr);
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
			*out() << t("Game_Warning_NoMessages") << std::endl;
			break;
		}
	}

	return std::unique_ptr<UserInterface>(new MainMenuUi());
}

void GameUi::visit(const JoinedRandomGameQueueMessage& message) {
	*out() << t("Game_Queued") << std::endl;

	clientstate()->gamestate()->session_id(message.session_id);
	clientstate()->gamestate()->self().player_id(message.player_id);
}
void print_quick_help(std::ostream& out) { out << std::endl << t("Game_QuickGuide") << std::endl; }
void GameUi::visit(const RandomGameFoundMessage& message) {
	*out() << t("Game_Queue_GameFound") << std::endl;

	// print out quick help
	print_quick_help(*out());
}
void GameUi::visit(const GameStartedMessage& message) {
	*out() << t("Game_Starting") << std::endl;

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

	*out() << t("Game_Ended", winner.name()) << std::endl;
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

static ActionChoice get_action_choice(std::ostream& out, std::istream& in) {
	out << t("Game_Choices") << std::endl;

	bool first_time = true;
	while (true) {
		out << t("Game_Prompt");

		std::string command;
		std::getline(in, command);

		if (ActionChoiceMap.count(command) == 1) {
			return ActionChoiceMap[command];
		}

		if (first_time) {
			out << std::endl << t("Game_ChoicesHelp") << std::endl;

			first_time = false;
		}
	}
}

void GameUi::action() {
	*out() << t("Game_TurnHint_You") << std::endl;

	ActionChoice choice = get_action_choice(*out(), *in());

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
	if (clientstate()->gamestate()->is_self_turn()) {
		*out() << t("Game_StaminaRegeneration_You", sta_gained) << std::endl;
	} else {
		*out() << t("Game_StaminaRegeneration_Opponent", clientstate()->gamestate()->opponent().name(), sta_gained) << std::endl;
	}

	print_status();

	if (clientstate()->gamestate()->is_self_turn()) {
		action();
	} else {
		*out() << t("Game_TurnHint_Opponent", clientstate()->gamestate()->opponent().name()) << std::endl;
	}
}

class SummaryFormatter {
	const Player& _self;
	const Player& _opponent;

	public:
	SummaryFormatter(const Player& self, const Player& opponent) : _self(self), _opponent(opponent) {}

	std::string format_action(const PlayerActionMessage& message) {
		std::ostringstream sb;

		bool is_caster_self = (message.caster_player_id == _self.player_id());

		if (message.action_failure) {
			// You want to attack, but fail to note that you have no stamina left.
			// You will have to wait until it regenerates next turn.

			// eräjorma wants to attack, but fails to note that she has no stamina left.
			// eräjorma will have to wait until it regenerates next turn.

			// You want to take a defensive position, but fail to note that you have no stamina left.
			// You will have to wait until it regenerates next turn.

			// eräjorma wants to take a defensive position, but fails to note that she has no stamina left.
			// eräjorma will have to wait until it regenerates next turn.

			if (message.action_request.ability.ability_type == PLAYERABILITYTYPE_ATTACK) {
				if (is_caster_self) {
					sb << t("Ability_Attack_NoStamina_You") << std::endl;
				} else {
					sb << t("Ability_Attack_NoStamina_Opponent", _opponent.name()) << std::endl;
				}
			} else if (message.action_request.ability.ability_type == PLAYERABILITYTYPE_DEFEND) {
				if (is_caster_self) {
					sb << t("Ability_Defend_NoStamina_You") << std::endl;
				} else {
					sb << t("Ability_Defend_NoStamina_Opponent", _opponent.name()) << std::endl;
				}
			}

			if (is_caster_self) {
				sb << t("Game_WaitUntilRegeneration_You") << std::endl;
			} else {
				sb << t("Game_WaitUntilRegeneration_Opponent", _opponent.name()) << std::endl;
			}
		} else {
			if (message.action_request.action_type == PLAYERACTIONTYPE_ABILITY) {
				if (message.action_request.ability.ability_type == PLAYERABILITYTYPE_ATTACK) {
					if (is_caster_self) {
						sb << "    >> " << t("Ability_Attack_Execute_You", _opponent.name(), message.effect_on_target.total_damage()) << std::endl;
					} else {
						sb << "    >> " << t("Ability_Attack_Execute_Opponent", _opponent.name(), message.effect_on_target.total_damage()) << std::endl;
					}
				} else if (message.action_request.ability.ability_type == PLAYERABILITYTYPE_DEFEND) {
					if (is_caster_self) {
						sb << "    >> " << t("Ability_Defend_Execute_You") << std::endl;
					} else {
						sb << "    >> " << t("Ability_Defend_Execute_Opponent", _opponent.name()) << std::endl;
					}
				} else {
					if (is_caster_self) {
						sb << "    >> " << t("Ability_Generic_Execute_You") << std::endl;
					} else {
						sb << "    >> " << t("Ability_Generic_Execute_Opponent", _opponent.name()) << std::endl;
					}
				}
			} else if (message.action_request.action_type == PLAYERACTIONTYPE_FORFEIT) {
				if (is_caster_self) {
					sb << "    >> " << t("Ability_Forfeit_Execute_You") << std::endl;
				} else {
					sb << "    >> " << t("Ability_Forfeit_Execute_Opponent", _opponent.name()) << std::endl;
				}
			} else if (message.action_request.action_type == PLAYERACTIONTYPE_IDLE) {
				if (is_caster_self) {
					sb << "    >> " << t("Ability_Idle_Execute_You") << std::endl;
				} else {
					sb << "    >> " << t("Ability_Idle_Execute_Opponent", _opponent.name()) << std::endl;
				}
			} else {
				if (is_caster_self) {
					sb << "    >> " << t("Ability_Generic_Execute_You") << std::endl;
				} else {
					sb << "    >> " << t("Ability_Generic_Execute_Opponent", _opponent.name()) << std::endl;
				}
			}
		}

		return sb.str();
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
		if (!message.effect_on_caster.empty()) {
			sb << "    " << stat_player(message.effect_on_caster, is_caster_self) << std::endl;
		}
		if (!message.effect_on_target.empty()) {
			sb << "    " << stat_player(message.effect_on_target, !is_caster_self) << std::endl;
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
	caster.apply(message.effect_on_caster);
	target.apply(message.effect_on_target);

	bool is_caster_self = (caster.player_id() == clientstate()->gamestate()->self().player_id());
	Player& self = is_caster_self ? caster : target;
	Player& opponent = is_caster_self ? target : caster;

	SummaryFormatter formatter(self, opponent);
	*out() << std::endl;
	*out() << formatter.format_action(message) << std::endl;

	std::string effects(formatter.format_effects(message));
	if (!effects.empty()) {
		*out() << effects << std::endl;
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
	*out() << std::endl;
	*out() << std::endl;

	// turn counter
	*out() << "    " << std::setw(31) << "[" << clientstate()->gamestate()->turn_counter() << "]";
	*out() << std::endl;

	// player names
	*out() << "    " << std::left << std::setw(29) << (clientstate()->gamestate()->self().name() + " (you)");
	*out() << "  vs  ";
	*out() << std::right << std::setw(29) << (clientstate()->gamestate()->opponent().name() + " (opponent)");
	*out() << std::endl;

	// player stats
	*out() << "    " << std::left << std::setw(30) << player_stats(clientstate()->gamestate()->self());
	*out() << "    " << std::right << std::setw(30) << player_stats(clientstate()->gamestate()->opponent());
	*out() << std::endl;

	// extra newline at end
	*out() << std::endl;
}