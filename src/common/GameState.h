#pragma once

#include "../common/CommonTypes.h"
#include "../common/Player.h"

class GameState {
	Player _player_a;
	Player _player_b;

	int _turn_counter;
	PlayerId _player_whose_turn_it_is;

	public:
	GameState() : _turn_counter(0) {
	}

	const Player& player_a() const { return _player_a; }
	Player& player_a() { return _player_a; }

	const Player& player_b() const { return _player_b; }
	Player& player_b() { return _player_b; }

	int turn_counter() const { return _turn_counter; }
	void turn_counter(int turn_counter) { _turn_counter = turn_counter; }

	Player& player_turn() {
		if (player_a().player_id() == _player_whose_turn_it_is) {
			return player_a();
		} else {
			return player_b();
		}
	}
	const Player& player_turn() const {
		if (player_a().player_id() == _player_whose_turn_it_is) {
			return player_a();
		} else {
			return player_b();
		}
	}
	void player_turn(PlayerId player_id) { _player_whose_turn_it_is = player_id; }

	Player& player_waiting() {
		if (player_a().player_id() == _player_whose_turn_it_is) {
			return player_b();
		} else {
			return player_a();
		}
	}
	const Player& player_waiting() const {
		if (player_a().player_id() == _player_whose_turn_it_is) {
			return player_b();
		} else {
			return player_a();
		}
	}

	void reset() { _turn_counter = 0; }
	void next_turn() {
		++_turn_counter;
		player_turn(player_waiting().player_id());
	}
};