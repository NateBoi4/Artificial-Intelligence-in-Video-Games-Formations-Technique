#include "EnemyUnit.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "WanderState.h"
#include "HealState.h"
#include "ChaseState.h"

void EnemyUnit::update(float elapsedTime)
{
	//heal based on proximity to heal Unit
	Unit* pHealUnit = gpGame->getUnitManager()->getUnit(HEAL_UNIT_ID);
	if (pHealUnit)
	{
		Vector2D diff = pHealUnit->getPosition() - getPosition();
		if (diff.getLengthSquared() <= gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_HEAL_RANGE_SQUARED).getUIntVal())
			heal(gpGame->getDataRepository()->getEntry(DataKeyEnum::HEAL_AMT).getIntVal());
	}

	//damage based on proximity to player Unit
	Unit* pPlayerUnit = gpGame->getUnitManager()->getUnit(PLAYER_UNIT_ID);
	if (pPlayerUnit)
	{
		Vector2D diff = pPlayerUnit->getPosition() - getPosition();
		if (diff.getLengthSquared() <= gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_DAMAGE_RANGE_SQUARED).getUIntVal())
			damage(gpGame->getDataRepository()->getEntry(DataKeyEnum::DAMAGE_AMT).getIntVal());
	}

	if (mHPs <= 0)
		mIsDead = true;

	//let base class do it's update
	GameUnit::update(elapsedTime);
	mpStateMachine->update(this); //Update ourselves
}

EnemyUnit::EnemyUnit(const Sprite& sprite)
	:GameUnit(sprite)
{
	//create State Machine
	mpStateMachine = new StateMachine();

	//Create the States
	StateMachineState* pWanderState = new WanderState(4);
	StateMachineState* pHealState = new HealState(5);
	StateMachineState* pChaseState = new ChaseState(6);

	//Create the Transitions
	StateTransition wanderTrans(WANDER_TRANSITION, 4);
	StateTransition healTrans(HEAL_TRANSITION, 5);
	StateTransition chaseTrans(CHASE_TRANSITION, 6);

	//add transitions to states
	pWanderState->addTransition(chaseTrans);

	pChaseState->addTransition(healTrans);

	pHealState->addTransition(wanderTrans);
	pHealState->addTransition(chaseTrans);

	//add states to the state machine
	mpStateMachine->addState(pWanderState);
	mpStateMachine->addState(pHealState);
	mpStateMachine->addState(pChaseState);

	mpStateMachine->setInitialStateID(4);
}

EnemyUnit::~EnemyUnit()
{
	delete mpStateMachine;
	mpStateMachine = NULL;
}
