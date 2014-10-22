#pragma once

#include <vector>
#include <memory>
#include <boost/date_time.hpp>
#include <cassert>

#include "Player.h"
#include "Action.h"

class GameState
{
	Player _player_a;
	Player _player_b;

	Player* _active_player;

	bool _has_ended = false;
	boost::posix_time::ptime _start_time;

	int _turn_counter;

public:
	Player& player_a() { return _player_a; }
	const Player& player_a() const { return _player_a; }

	Player& player_b() { return _player_b; }
	const Player& player_b() const { return _player_b; }

	Player* active_player() { return _active_player; }
	const Player* active_player() const { return _active_player; }

	void active_player(Player* player) {
		assert(player == &_player_a || player == &_player_b);

		_active_player = player;
	}

	Player* target_player() { return _active_player == &_player_a ? &_player_b : &_player_a; }
	const Player* target_player() const { return _active_player == &_player_a ? &_player_b : &_player_a; }

	bool has_ended() const { return _has_ended; }

	const boost::posix_time::ptime& start_time() const { return _start_time; }

	void next_turn() {
		active_player(target_player());
		++_turn_counter;
	}

	int turn_counter() const { return _turn_counter; }

	GameState();
	~GameState();

	void apply(const Action& action);
};

