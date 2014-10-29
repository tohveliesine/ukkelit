#pragma once

#include <memory>
#include <iostream>
#include <cassert>
#include <string>

#include "../client/ClientState.h"
#include "../common/ServerCommunication.h"
#include "../common/Translations.h"

class UserInterface {
	private:
	std::istream* _in;
	std::ostream* _out;
	std::shared_ptr<ClientState> _clientstate;

	protected:
	UserInterface() {}

	virtual std::unique_ptr<UserInterface> run_ui() { return nullptr; }

	std::istream* in() { return _in; }
	std::ostream* out() { return _out; }

	std::shared_ptr<ClientState> clientstate() { return _clientstate; }

	std::string prompt(const std::string& default_value) {
		assert(in() != nullptr);
		assert(out() != nullptr);

		*out() << "[" << default_value << "] ?> ";
		out()->flush();

		std::string value;
		std::getline(*in(), value);

		return value.empty() ? default_value : value;
	}

	public:
	virtual ~UserInterface(){};

	std::unique_ptr<UserInterface> run(std::shared_ptr<ClientState> clientstate, std::istream* in,
	                                   std::ostream* out) {
		assert(clientstate != nullptr);
		assert(in != nullptr);
		assert(out != nullptr);

		_clientstate = clientstate;
		_in = in;
		_out = out;

		return run_ui();
	}
};

class StartupUi : public UserInterface {
	protected:
	virtual std::unique_ptr<UserInterface> run_ui();

	public:
	StartupUi() {}
	~StartupUi() {}
};

class FirstTimeUi : public UserInterface {
	protected:
	virtual std::unique_ptr<UserInterface> run_ui();

	public:
	FirstTimeUi() {}
	~FirstTimeUi() {}
};

class MainMenuUi : public UserInterface {
	protected:
	virtual std::unique_ptr<UserInterface> run_ui();

	public:
	MainMenuUi() {}
	~MainMenuUi() {}
};

class ConnectRandomGameUi : public UserInterface {
	protected:
	virtual std::unique_ptr<UserInterface> run_ui();

	public:
	ConnectRandomGameUi() {}
	~ConnectRandomGameUi() {}
};

class GameUi : public UserInterface, public ServerCommunicationVisitor {
	void action();

	protected:
	virtual std::unique_ptr<UserInterface> run_ui();

	public:
	GameUi() {}
	~GameUi() {}

	void visit(const JoinedRandomGameQueueMessage& message);
	void visit(const RandomGameFoundMessage& message);
	void visit(const GameStartedMessage& message);
	void visit(const GameEndedMessage& message);
	void visit(const TurnChangedMessage& message);
	void visit(const PlayerActionMessage& message);

	private:
	void print_status();
};

class HelpUi : public UserInterface {
	protected:
	virtual std::unique_ptr<UserInterface> run_ui();

	public:
	HelpUi() {}
	~HelpUi() {}
};