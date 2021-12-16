#include "WanderState.h"
#include "Game.h"
#include "GameUnit.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "System.h"
#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


using namespace std;

void WanderState::onEntrance()
{
	//srand(time(NULL));
	randX = rand() % (int)gpGame->getGraphicsSystem()->getDisplayWidth();
	randY = rand() % (int)gpGame->getGraphicsSystem()->getDisplayHeight();
	//cout << (int)gpGame->getGraphicsSystem()->getDisplayWidth() / 32 << endl;
	//cout << (int)gpGame->getGraphicsSystem()->getDisplayHeight() / 32 << endl;
	//std::cout << "Searching for player...\n";
}

void WanderState::onExit()
{
}

StateTransition* WanderState::update(EnemyUnit* pUnit)
{
	if (pUnit) 
	{
		scale();
		//std::cout << "Searching for player...\n";
		//Set the red arrow sprite
		gpGame->getUnitManager()->getUnit(pUnit->getID())->setSprite(*gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID));
		//Set the Unit to wander
		Vector2D center((int)gpGame->getGraphicsSystem()->getDisplayWidth() / 2, (int)gpGame->getGraphicsSystem()->getDisplayHeight() / 2);
		Vector2D centerLeft(0, (int)gpGame->getGraphicsSystem()->getDisplayHeight() / 2);
		Vector2D centerTop((int)gpGame->getGraphicsSystem()->getDisplayWidth() / 2, 0);
		cross = (int)pUnit->getID() % 2;
		if (cross) 
		{
			centerLeft += (Vector2D((float)randX, 0.0f) * mScale);
			gpGame->getUnitManager()->getUnit(pUnit->getID())->setSteering(Steering::ARRIVE, centerLeft);
		}
		else 
		{
			centerTop += (Vector2D(0.0f, (float)randY) * mScale);
			gpGame->getUnitManager()->getUnit(pUnit->getID())->setSteering(Steering::ARRIVE, centerTop);
		}
		Unit* pPlayerUnit = gpGame->getUnitManager()->getUnit(PLAYER_UNIT_ID); //player unit
		if (pPlayerUnit) 
		{
			//Distance from Player Unit to this Unit
			Vector2D diff = pPlayerUnit->getPosition() - pUnit->getPositionComponent()->getPosition();
			if (diff.getLengthSquared() <= gpGame->getDataRepository()->getEntry(DataKeyEnum::CLOSE_DIST).getIntVal()) 
			{
				//If we are close to the player, as defined in data.txt, transition to the chase state.
				map<TransitionType, StateTransition>::iterator iter = mTransitions.find(CHASE_TRANSITION);
				if (iter != mTransitions.end())//found?
				{
					return &(iter->second);
				}
				std::cout << "ERROR - no matching transition!\n";
			}
		}
		else 
		{
			if (gpGame->getSystem()->getInputSystem()->isKeyPressed(InputSystem::ONE_KEY))
			{
				//If we are close to the player, as defined in data.txt, transition to the chase state.
				map<TransitionType, StateTransition>::iterator iter = mTransitions.find(CHASE_TRANSITION);
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

void WanderState::scale()
{
	if (gpGame->getSystem()->getInputSystem()->isKeyPressed(InputSystem::FOUR_KEY))
		mScale += 0.1f;
	else if (gpGame->getSystem()->getInputSystem()->isKeyPressed(InputSystem::FIVE_KEY))
		mScale -= 0.1f;
	//cout << mScale << endl;
}
