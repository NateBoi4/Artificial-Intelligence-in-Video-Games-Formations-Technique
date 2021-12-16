#pragma once

#include "StateMachine.h"

class WanderState : public StateMachineState
{
public:
	WanderState(const SM_idType& id) :StateMachineState(id) {};

	virtual void onEntrance();
	virtual void onExit();
	virtual StateTransition* update(EnemyUnit* pUnit);

	void scale();
private:
	Vector2D targetPos;
	bool cross = true;
	int randX = 0;
	int randY = 0;

	float mScale = 1.0f;
};

