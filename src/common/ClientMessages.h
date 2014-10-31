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
	virtual ~ClientMessage() {}
};

struct JoinRandomGameQueueClientMessage : public ClientMessage {
	std::string alias_name;

	void accept(ClientCommunicationVisitor& visitor) const { visitor.visit(*this); }
};
struct PlayerActionRequestClientMessage : public ClientMessage {
	SessionId session_id;
	std::string ability_id;

	void accept(ClientCommunicationVisitor& visitor) const { visitor.visit(*this); }
};

enum ClientMessageType {
	CLIENTMESSAGETYPE_JOINRANDOMGAMEQUEUE,
	CLIENTMESSAGETYPE_PLAYERACTIONREQUEST,
};