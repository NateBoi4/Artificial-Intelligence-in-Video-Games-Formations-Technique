#pragma once

#include "GameUnit.h"
#include "StateMachine.h"

class EnemyUnit :public GameUnit
{
public:
	void update(float elapsedTime);
	int getHP() { return mHPs; }; //Return our current HP

protected:
	EnemyUnit(const Sprite& sprite);
	virtual ~EnemyUnit();

	EnemyUnit(EnemyUnit&);//invalidate copy constructor
	void operator=(EnemyUnit&);//invalidate assignment operator

	friend class UnitManager;
private:
	StateMachine* mpStateMachine; //each unit has its own state machine

};
