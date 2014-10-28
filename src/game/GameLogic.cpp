#include <cassert>

#include "GameLogic.h"
#include "../common/ActionEffect.h"

GameLogic::GameLogic() {}

GameLogic::~GameLogic() {}

void GameLogic::visit(const JoinRandomGameQueueClientMessage& message) {
	assert(_server_state.client_communication() != nullptr);

	std::shared_ptr<JoinedRandomGameQueueMessage> reply_joined_queue(new JoinedRandomGameQueueMessage());
	reply_joined_queue->ongoing_games_count = 0;
	reply_joined_queue->player_id = std::rand();
	reply_joined_queue->session_id = std::rand();
	_server_state.client_communication()->send_message(reply_joined_queue);

	std::shared_ptr<RandomGameFoundMessage> reply_game_found(new RandomGameFoundMessage());
	_server_state.client_communication()->send_message(reply_game_found);

	// set up game state
	server_state().gamestate().player_a().name(message.alias_name);
	server_state().gamestate().player_a().player_id(reply_joined_queue->player_id);
	server_state().gamestate().player_a_sessionid(reply_joined_queue->session_id);

	server_state().gamestate().player_b().player_id(std::rand());
	server_state().gamestate().player_b().name("computer ai");
	server_state().gamestate().player_b_sessionid(std::rand());

	// announce it started
	announce_game_started();

	// set up turn
	server_state().gamestate().player_turn(server_state().gamestate().player_a().player_id());

	// and announce turn
	announce_turn();
}

void GameLogic::visit(const PlayerActionRequestClientMessage& message) {
	assert(_server_state.client_communication() != nullptr);

	if (message.session_id != server_state().gamestate().session_turn()) {
		// disconnect!
		assert("disco" && false);
	}

	Player& caster = server_state().gamestate().player_turn();
	Player& target = server_state().gamestate().player_waiting();

	std::shared_ptr<PlayerActionMessage> effect_message(new PlayerActionMessage());
	effect_message->action_request = message;
	effect_message->caster_player_id = server_state().gamestate().player_turn().player_id();
	effect_message->target_player_id = server_state().gamestate().player_waiting().player_id();

	if (message.action_type == PLAYERACTIONTYPE_ABILITY) {
		ActionEffect effect_on_caster;
		ActionEffect effect_on_target;
		if (message.ability.ability_type == PLAYERABILITYTYPE_ATTACK) {
			effect_on_caster.effect_on_stamina(-4);
			effect_on_target.deal_damage(6, target);
		} else if (message.ability.ability_type == PLAYERABILITYTYPE_DEFEND) {
			effect_on_caster.effect_on_stamina(-2);
			effect_on_caster.effect_on_defense(4);
		}
		
		// check that there is enough stamina
		if (server_state().gamestate().player_turn().stamina() + effect_on_caster.effect_on_stamina() < 0) {
			// not enough stamina!
			effect_message->action_failure = true;
			effect_message->action_failure_reason = PLAYERACTIONFAILUREREASON_NOTENOUGHSTAMINA;
		} else {
			// apply effects to game state
			server_state().gamestate().player_turn().apply(effect_on_caster);
			server_state().gamestate().player_waiting().apply(effect_on_target);
			effect_message->effect_on_caster = effect_on_caster;
			effect_message->effect_on_target = effect_on_target;
		}

		// send message
		server_state().client_communication()->send_message(effect_message);

		// check for game over
		if (server_state().gamestate().player_a().is_alive() && server_state().gamestate().player_b().is_alive()) {
			// no one dead, game continues
			announce_next_turn();
		} else {
			// death for one of the players
			if (!server_state().gamestate().player_a().is_alive()) {
				// player a died, winner is player b
				server_state().gamestate().winner(server_state().gamestate().player_b().player_id());
			} else {
				// player b died, winner is player a
				server_state().gamestate().winner(server_state().gamestate().player_a().player_id());
			}

			announce_game_ended();
		}
	} else if (message.action_type == PLAYERACTIONTYPE_IDLE) {
		server_state().client_communication()->send_message(effect_message);

		announce_next_turn();
	} else if (message.action_type == PLAYERACTIONTYPE_FORFEIT) {
		server_state().client_communication()->send_message(effect_message);

		server_state().gamestate().winner(target.player_id());

		announce_game_ended();
	} else {
		assert("unknown action type" && false);
	}
}

void GameLogic::announce_game_ended() {
	assert(server_state().gamestate().winner() != 0);

	std::shared_ptr<GameEndedMessage> game_ended_message(new GameEndedMessage());
	game_ended_message->winner_player_id = server_state().gamestate().winner();
	server_state().client_communication()->send_message(game_ended_message);
}

void GameLogic::announce_next_turn() {
	server_state().gamestate().next_turn();

	announce_turn();

	// ai stuff: always just attempt to attack
	Player &ai = server_state().gamestate().player_b();
	if (&server_state().gamestate().player_turn() == &ai) {
		SessionId ai_session_id = server_state().gamestate().player_b_sessionid();
		ai.attack_request(ai_session_id)->accept(*this);
	}
}

void GameLogic::announce_turn() {
	std::shared_ptr<TurnChangedMessage> turn_changed_message(new TurnChangedMessage());
	turn_changed_message->player_id = server_state().gamestate().player_turn().player_id();
	turn_changed_message->turn_number = server_state().gamestate().turn_counter();
	if (server_state().gamestate().turn_counter() > 0) {
		turn_changed_message->effect_on_player.effect_on_stamina(+1);
		server_state().gamestate().player_turn().apply(turn_changed_message->effect_on_player);
	}
	server_state().client_communication()->send_message(turn_changed_message);
}

void GameLogic::announce_game_started() {
	assert(server_state().gamestate().player_a_sessionid() != 0);
	assert(server_state().gamestate().player_b_sessionid() != 0);

	std::shared_ptr<GameStartedMessage> reply_game_started(new GameStartedMessage());
	reply_game_started->player_a = server_state().gamestate().player_a();
	reply_game_started->player_b = server_state().gamestate().player_b();
	server_state().client_communication()->send_message(reply_game_started);
}