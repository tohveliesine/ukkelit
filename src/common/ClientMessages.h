#pragma once

#include <memory>
#include <string>
#include <iostream>

struct JoinRandomGameQueueClientMessage;
struct PlayerActionRequestClientMessage;

class ClientCommunicationVisitor {
	public:
	virtual void visit(const JoinRandomGameQueueClientMessage& message) = 0;
	virtual void visit(const PlayerActionRequestClientMessage& message) = 0;
};

struct ClientMessage {
	virtual void accept(ClientCommunicationVisitor& visitor) const = 0;
};

struct JoinRandomGameQueueClientMessage : public ClientMessage {
	std::string alias_name;

	void accept(ClientCommunicationVisitor& visitor) const { visitor.visit(*this); }
};

enum PlayerActionType {
	PLAYERACTIONTYPE_ABILITY,
	PLAYERACTIONTYPE_IDLE,
	PLAYERACTIONTYPE_FORFEIT,
};

enum PlayerAbilityType {
	PLAYERABILITYTYPE_ATTACK,
	PLAYERABILITYTYPE_DEFEND,
};

struct PlayerAbility {
	PlayerAbilityType ability_type;
};

struct PlayerActionRequestClientMessage : public ClientMessage {
	SessionId session_id;
	PlayerActionType action_type;
	PlayerAbility ability;

	void accept(ClientCommunicationVisitor& visitor) const { visitor.visit(*this); }

	PlayerActionRequestClientMessage() {}

	PlayerActionRequestClientMessage(PlayerAbilityType ability_type) {
		ability.ability_type = ability_type;
	}
};

enum ClientMessageType {
	CLIENTMESSAGETYPE_JOINRANDOMGAMEQUEUE,
	CLIENTMESSAGETYPE_PLAYERACTIONREQUEST,
};