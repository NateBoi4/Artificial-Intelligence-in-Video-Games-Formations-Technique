#include "ChaseState.h"
#include "Game.h"
#include "GameUnit.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "System.h"
#include <iostream>
#include <math.h>

using namespace std;

void ChaseState::onEntrance()
{
}

void ChaseState::onExit()
{
}

StateTransition* ChaseState::update(EnemyUnit* pUnit)
{
	if (pUnit) 
	{
		scale();
		//std::cout << "Chasing the player...\n";
		//Set the magenta arrow sprite.
		gpGame->getUnitManager()->getUnit(pUnit->getID())->setSprite(*gpGame->getSpriteManager()->getSprite(AI_CHASE_ICON_SPRITE_ID));
		Vector2D center((int)gpGame->getGraphicsSystem()->getDisplayWidth() / 2, (int)gpGame->getGraphicsSystem()->getDisplayHeight() / 2);
		Vector2D ring((float)cos((int)pUnit->getID()), (float)sin((int)pUnit->getID()));
		ring *= mScale;
		Vector2D circle = ring + center;
		gpGame->getUnitManager()->getUnit(pUnit->getID())->setSteering(Steering::ARRIVE, circle);
		Unit* pPlayerUnit = gpGame->getUnitManager()->getUnit(PLAYER_UNIT_ID); //player unit
		if (pPlayerUnit) 
		{
			//Set this unit to seek out the player unit
			gpGame->getUnitManager()->getUnit(pUnit->getID())->setSteering(Steering::SEEK, pPlayerUnit->getPosition());
			if (pUnit->getHP() <= gpGame->getDataRepository()->getEntry(DataKeyEnum::HURT_AMT).getIntVal())
			{
				//If we are hurt, as defined in data.txt, then transition to the heal state.
				map<TransitionType, StateTransition>::iterator iter = mTransitions.find(HEAL_TRANSITION);
				if (iter != mTransitions.end())//found?
				{
					return &(iter->second);
				}
				std::cout << "ERROR - no matching transition!\n";
			}
		}
		else
		{
			if (gpGame->getSystem()->getInputSystem()->isKeyPressed(InputSystem::TWO_KEY))
			{
				//If we are close to the player, as defined in data.txt, transition to the chase state.
				map<TransitionType, StateTransition>::iterator iter = mTransitions.find(HEAL_TRANSITION);
				if (iter != mTransitions.end())//found?
				{
					return &(iter->second);
				}
				std::cout << "ERROR - no matching transition!\n";
			}
		}
	}
	return NULL;
}

void ChaseState::scale()
{
	if (gpGame->getSystem()->getInputSystem()->isKeyPressed(InputSystem::FOUR_KEY))
		mScale += 50.0f;
	else if (gpGame->getSystem()->getInputSystem()->isKeyPressed(InputSystem::FIVE_KEY))
		mScale -= 50.0f;
	//cout << mScale << endl;
}
