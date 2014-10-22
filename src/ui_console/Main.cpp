#include <iostream>
#include <memory>
#include <chrono>

#include "Main.h"
#include "../common/GameState.h"
#include "../common/Player.h"

Main::Main() {
}

Main::~Main() {
}

void get_user_action(const std::shared_ptr<Player> player, ActionRequest &action_request)
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

		int option;
		std::cin >> option;

		// decode
		switch (option) {
		case 1:
			player->hero().attackRequest(action_request);
			success = true;
			break;
		case 2:
			player->hero().defendRequest(action_request);
			success = true;
			break;
		case 3:
			action_request.action_type(ACTIONTYPE_IDLE);
			success = true;
			break;
		default:
			std::cerr << "Please give a proper choice: number 1 to 3" << std::endl;
			break;
		}
	}
}

void get_action_effect(const GameState &gamestate,
	const std::shared_ptr<Player> player, const ActionRequest& action_request, Action& action)
{
	action.request(action_request);
	switch (action.request().action_type())
	{
	case ACTIONTYPE_ATTACK:
		action.effect_for_caster().stamina(-4);
		action.effect_for_target().healthpoints(-6);
		break;
	case ACTIONTYPE_DEFEND:
		action.effect_for_caster().stamina(-2);
		action.effect_for_caster().defense(+3);
		break;
	}
}


void Main::Run() {
	std::cout << "Ukkelit" << std::endl << std::endl;

	// set up the game with two players
	GameState gamestate;
	gamestate.players().push_back(std::make_shared<Player>(L"Modren Man"));
	gamestate.players().push_back(std::make_shared<Player>(L"Tundra"));
	gamestate.active_player(gamestate.players().at(0));

	// print game state
	std::cout << "Game started at " <<
		boost::posix_time::to_simple_string(gamestate.start_time().time_of_day()) << std::endl;

	// 
	while (!gamestate.has_ended()) {
		// print hero stats
		for (auto player : gamestate.players())
		{
			std::wcout << "Player " << player->name() << "'s hero ("
				<< "+" << player->hero().healthpoints() << " hp, "
				<< "+" << player->hero().defense() << " def, "
				<< "+" << player->hero().stamina() << " sta)"
				<< std::endl;
		}
		std::cout << std::endl;

		// print whose turn it is
		auto player = gamestate.active_player();
		std::wcout << "It is " << player->name() << "'s turn" << std::endl;

		// choose the next course of action
		ActionRequest actionRequest;
		get_user_action(player, actionRequest);

		// calculate effects of attack
		Action action;
		get_action_effect(gamestate, player, actionRequest, action);

		// apply effects
		gamestate.apply(action);

		// next turn
	}

	std::cout << "Game ended." << std::endl << "Press any key to continue." << std::endl;
	std::cin.get();
}