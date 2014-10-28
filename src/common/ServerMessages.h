#pragma once

#include "Player.h"
#include "ClientMessages.h"
#include "ActionEffect.h"

struct JoinedRandomGameQueueMessage;
struct RandomGameFoundMessage;
struct GameStartedMessage;
struct GameEndedMessage;
struct TurnChangedMessage;
struct PlayerActionMessage;

// defines client's callbacks.
// used by clients to handle communication traffic from servers.
class ServerCommunicationVisitor {
	public:
	virtual void visit(const JoinedRandomGameQueueMessage& message) = 0;
	virtual void visit(const RandomGameFoundMessage& message) = 0;
	virtual void visit(const GameStartedMessage& message) = 0;
	virtual void visit(const GameEndedMessage& message) = 0;
	virtual void visit(const TurnChangedMessage& message) = 0;
	virtual void visit(const PlayerActionMessage& message) = 0;
};

struct ServerMessage {
	virtual void accept(ServerCommunicationVisitor& visitor) const = 0;
};

struct JoinedRandomGameQueueMessage : public ServerMessage {
	// number of games started on this server, and not yet ended (or cleaned)
	int ongoing_games_count;

	// session id used to identify the receiving client and map it to an ongoing game.
	// it is supposed to stay private so that no one else can interfere with your game.
	// (session hijacking is another matter)
	SessionId session_id;

	// player id is public knowledge.
	// identifies the receiving client's player with this id.
	PlayerId player_id;

	void accept(ServerCommunicationVisitor& visitor) const { visitor.visit(*this); }
};

struct RandomGameFoundMessage : public ServerMessage {
	void accept(ServerCommunicationVisitor& visitor) const { visitor.visit(*this); }
};

struct GameStartedMessage : public ServerMessage {
	Player player_a;
	Player player_b;

	void accept(ServerCommunicationVisitor& visitor) const { visitor.visit(*this); }
};

struct GameEndedMessage : public ServerMessage {
	// the game was won by the player with this identifier
	PlayerId winner_player_id;

	void accept(ServerCommunicationVisitor& visitor) const { visitor.visit(*this); }
};

struct TurnChangedMessage : public ServerMessage {
	// turn changed to the player with this identifier
	PlayerId player_id;
	int turn_number;

	// the player whose turn it is gets +2 sta every turn.
	ActionEffect effect_on_player;

	void accept(ServerCommunicationVisitor& visitor) const { visitor.visit(*this); }
};

enum PlayerActionFailureReason {
	PLAYERACTIONFAILUREREASON_NOFAILURE,
	PLAYERACTIONFAILUREREASON_NOTENOUGHSTAMINA,
};

struct PlayerActionMessage : public ServerMessage {
	// original request sent in by the client
	PlayerActionRequestClientMessage action_request;

	bool action_failure;
	PlayerActionFailureReason action_failure_reason;

	PlayerId caster_player_id;
	PlayerId target_player_id;

	ActionEffect effect_on_caster;
	ActionEffect effect_on_target;

	PlayerActionMessage() : action_failure(false), action_failure_reason(PLAYERACTIONFAILUREREASON_NOFAILURE) {}
	void accept(ServerCommunicationVisitor& visitor) const { visitor.visit(*this); }
};