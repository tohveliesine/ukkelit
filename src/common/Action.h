#pragma once

#include <memory>

#include "ActionRequest.h"
#include "ActionEffect.h"

class Action
{
	ActionEffect _effect_for_caster;
	ActionEffect _effect_for_target;
	ActionRequest _request;

public:
	const ActionEffect& effect_for_caster() const { return _effect_for_caster; }
	ActionEffect& effect_for_caster() { return _effect_for_caster; }

	const ActionEffect& effect_for_target() const { return _effect_for_target; }
	ActionEffect& effect_for_target() { return _effect_for_target; }

	const ActionRequest& request() const { return _request; }
	ActionRequest& request() { return _request; }
	void request(const ActionRequest& request) { _request = request; }

	Action();
	~Action();
};

