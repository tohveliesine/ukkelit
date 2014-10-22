#pragma once

#include "ActionType.h"

class ActionRequest
{
	ActionType _action_type;

public:
	ActionType action_type() const { return _action_type; }
	void action_type(ActionType action_type) { _action_type = action_type; }

	ActionRequest();
	~ActionRequest();
};

