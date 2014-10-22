#pragma once

#include <vector>
#include <memory>
#include <boost/date_time.hpp>
#include "Player.h"
#include "Action.h"

class GameState
{
	std::vector< std::shared_ptr<Player> > _players;
	std::shared_ptr<Player> _active_player;

	bool _has_ended = false;
	boost::posix_time::ptime _start_time;

public:
	std::vector< std::shared_ptr<Player> >& players() { return _players; }
	const std::vector< std::shared_ptr<Player> >& players() const { return _players; }

	std::shared_ptr<Player> active_player() { return _active_player; }
	const std::shared_ptr<Player> active_player() const { return _active_player; }
	void active_player(std::shared_ptr<Player> active_player) { _active_player = active_player; }

	bool has_ended() const { return _has_ended; }

	const boost::posix_time::ptime& start_time() const { return _start_time; }

	GameState();
	~GameState();

	void apply(const Action& action);
};

