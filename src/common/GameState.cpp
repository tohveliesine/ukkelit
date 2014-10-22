#include <boost/date_time.hpp>
#include "GameState.h"

GameState::GameState()
	: _start_time(boost::posix_time::second_clock::universal_time())
{
}

GameState::~GameState()
{
}

void GameState::apply(const Action& action)
{
	Player* caster = _active_player.get();
	Player* target = (caster == _players.at(0).get()) ? _players.at(1).get() : _players.at(0).get();

	caster->hero().apply(action.effect_for_caster());
	target->hero().apply(action.effect_for_target());
}
