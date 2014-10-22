#include <iostream>
#include <memory>
#include <chrono>

#include "Main.h"
#include "../common/_ver.h"
#include "../common/GameState.h"
#include "../common/Player.h"

Main::Main() {
}

Main::~Main() {
}

void get_user_action(ActionRequest &action_request)
{
	std::cout << "Choose action:" << std::endl;
	std::cout << "1. Attack (-4 sta) (-6 hp)" << std::endl;
	std::cout << "2. Defend (-2 sta, +3 def) ()" << std::endl;
	std::cout << "3. Do nothing" << std::endl;
	std::cout << std::endl;
	std::cout << "Hero will regain 3 sta next turn." << std::endl;
	std::cout << std::endl;

	bool success = false;
	while (!success) {
		std::cout << "Enter your choice as a number: " << std::flush;

		// get input
		std::string line;
		std::getline(std::cin, line);

		// parse
		std::istringstream ss(line);
		int option;
		ss >> option;
		if (ss.bad() || ss.fail()) {
			std::cerr << "Please give a number." << std::endl;
			continue;
		}

		// decode
		switch (option) {
		case 1:
			action_request.caster()->hero().attackRequest(action_request);
			success = true;
			break;
		case 2:
			action_request.caster()->hero().defendRequest(action_request);
			success = true;
			break;
		case 3:
			action_request.action_type(ACTIONTYPE_IDLE);
			success = true;
			break;
		default:
			std::cerr << "Please choose between 1 and 3" << std::endl;
			break;
		}
	}
}

void get_action_effect(const GameState &gamestate,
	const ActionRequest& action_request, Action& action)
{
	action.request(action_request);
	switch (action.request().action_type())
	{
	case ACTIONTYPE_ATTACK:
		action.effect_for_caster().effect_on_stamina(-4);
		action.effect_for_target().deal_damage(6, action_request.target()->hero());
		break;
	case ACTIONTYPE_DEFEND:
		action.effect_for_caster().effect_on_stamina(-2);
		action.effect_for_caster().effect_on_defense(+3);
		break;
	}
}

void print_hero_stats(Player& player, bool active) {
	std::wcout
		<< (active ? "> " : "  ")
		<< "Player " << player.name() << "'s hero ("
		<< "+" << player.hero().healthpoints() << " hp, "
		<< "+" << player.hero().defense() << " def, "
		<< "+" << player.hero().stamina() << " sta)"
		<< std::endl;
}

void print_effects(const Player* player, const ActionEffect& effect) {
	std::wcout << "Player " << player->name() << "'s hero's effects: "
		<< effect.effect_on_healthpoints() << " hp, "
		<< effect.effect_on_defense() << " def, "
		<< effect.effect_on_stamina() << " sta"
		<< std::endl;
}

void print_action(const Action& action) {
	switch (action.request().action_type()) {
	case ACTIONTYPE_ATTACK:
		std::wcout
			<< action.request().caster()->name() << " attacks " << action.request().target()->name()
			<< ". Deals " << action.effect_for_target().total_damage() << " damage." << std::endl;
		break;
	case ACTIONTYPE_DEFEND:
		std::wcout
			<< action.request().caster()->name() << " chooses to defend the position. "
			<< "Gains +" << action.effect_for_caster().effect_on_defense() << " defense." << std::endl;
		break;
	case ACTIONTYPE_IDLE:
		std::wcout 
			<< action.request().caster()->name() << " does nothing." << std::endl;
		break;
	}
	
	print_effects(action.request().caster(), action.effect_for_caster());
	print_effects(action.request().target(), action.effect_for_target());
}

void Main::run() {
	std::cout << "Ukkelit v" << UKKELIT_VERSION << std::endl << std::endl;

	// set up the game with two players
	GameState gamestate;
	gamestate.player_a().name(L"Modren Man");
	gamestate.player_b().name(L"Tundra");

	// print game state
	std::cout << "Game started at " <<
		boost::posix_time::to_simple_string(gamestate.start_time().time_of_day()) << std::endl;

	// 
	while (!gamestate.has_ended()) {
		// print hero stats
		std::cout << "==== Turn " << gamestate.turn_counter() << " ====" << std::endl;
		print_hero_stats(gamestate.player_a(), &gamestate.player_a() == gamestate.active_player());
		print_hero_stats(gamestate.player_b(), &gamestate.player_b() == gamestate.active_player());
		std::cout << std::endl;

		auto caster = gamestate.active_player();

		// choose the next course of action
		ActionRequest actionRequest;
		actionRequest.caster(gamestate.active_player());
		actionRequest.target(gamestate.target_player());
		get_user_action(actionRequest);

		// calculate effects of attack
		Action action;
		get_action_effect(gamestate, actionRequest, action);

		// apply and print effects
		gamestate.apply(action);
		print_action(action);
		std::cout << std::endl;

		// see if game ended
		if (!gamestate.player_a().hero().is_alive() && !gamestate.player_b().hero().is_alive()) {
			std::cout << "Game ended in a tie." << std::endl;
			break;
		}
		else if (!gamestate.player_a().hero().is_alive()) {
			std::wcout << gamestate.player_b().name() << " won!" << std::endl;
			break;
		}
		else if (!gamestate.player_b().hero().is_alive()) {
			std::wcout << gamestate.player_a().name() << " won!" << std::endl;
			break;
		}

		// next turn
		gamestate.next_turn();
	}

	std::cout << "Game ended." << std::endl << "Press RETURN to continue." << std::endl;
	std::string s;
	std::getline(std::cin, s);
}