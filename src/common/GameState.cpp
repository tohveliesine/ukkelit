#include <boost/date_time.hpp>
#include "GameState.h"

GameState::GameState()
	: _start_time(boost::posix_time::second_clock::universal_time()),
	_turn_counter(1)
{
	_active_player = &_player_a;
}

GameState::~GameState()
{
}

void GameState::apply(const Action& action)
{
	Player* caster = _active_player;
	Player* target = target_player();

	caster->hero().apply(action.effect_for_caster());
	target->hero().apply(action.effect_for_target());
}
