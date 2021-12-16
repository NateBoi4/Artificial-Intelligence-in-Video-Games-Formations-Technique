#pragma once

#include "StateMachine.h"

class ChaseState : public StateMachineState
{
public:
	ChaseState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update(EnemyUnit* pUnit);

	void scale();
private:
	float mScale = 200.0f;
};

