#pragma once

#include "StateMachine.h"

class HealState : public StateMachineState
{
public:
	HealState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update(EnemyUnit* pUnit);

	void scale();
private:
	float mScale = 5.0f;
};

