#include "HealState.h"
#include "Game.h"
#include "GameUnit.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"
#include "System.h"
#include <iostream>

using namespace std;

void HealState::onEntrance()
{
}

void HealState::onExit()
{
}

StateTransition* HealState::update(EnemyUnit* pUnit)
{
	if (pUnit)
	{
		scale();
		//std::cout << "Healing the Unit...\n";
		//Set the green arrow sprite
		gpGame->getUnitManager()->getUnit(pUnit->getID())->setSprite(*gpGame->getSpriteManager()->getSprite(AI_HEAL_ICON_SPRITE_ID));
		Vector2D centerTop((int)gpGame->getGraphicsSystem()->getDisplayWidth() / 2, 0);
		bool evenOdds = (int)pUnit->getID() % 2;
		if (evenOdds) 
		{
			Vector2D pos(-(float)pUnit->getID(), (float)pUnit->getID());
			pos *= mScale;
			Vector2D left = centerTop + pos;
			//left *= 10.0f;
			gpGame->getUnitManager()->getUnit(pUnit->getID())->setSteering(Steering::ARRIVE, left);
		}
		else 
		{
			Vector2D pos((float)pUnit->getID(), (float)pUnit->getID());
			pos *= mScale;
			Vector2D right = centerTop + pos;
			//right *= 10.0f;
			gpGame->getUnitManager()->getUnit(pUnit->getID())->setSteering(Steering::ARRIVE, right);
		}
		Unit* pPlayerUnit = gpGame->getUnitManager()->getUnit(PLAYER_UNIT_ID); //player unit
		Unit* pHealUnit = gpGame->getUnitManager()->getUnit(HEAL_UNIT_ID); //heal unit
		if (pHealUnit && pPlayerUnit)
		{
			//Set this Unit to arrive at the heal unit location
			gpGame->getUnitManager()->getUnit(pUnit->getID())->setSteering(Steering::ARRIVE, pHealUnit->getPosition());
			if (pUnit->getHP() >= gpGame->getDataRepository()->getEntry(DataKeyEnum::HEALED_AMT).getIntVal())
			{
				//If we are healed, as defined in data.txt, then:
				if (pPlayerUnit) 
				{
					Vector2D diff = pPlayerUnit->getPosition() - pUnit->getPositionComponent()->getPosition();
					if (diff.getLengthSquared() <= gpGame->getDataRepository()->getEntry(DataKeyEnum::CLOSE_DIST).getIntVal())
					{
						//If we are close to the player unit, as defined in data.txt, then transition back to the chase state.
						map<TransitionType, StateTransition>::iterator iter = mTransitions.find(CHASE_TRANSITION);
						if (iter != mTransitions.end())//found?
						{
							return &(iter->second);
						}
						std::cout << "ERROR - no matching transition!\n";
					}
					else 
					{
						//Else we are not close so transition to the wander state.
						map<TransitionType, StateTransition>::iterator iter = mTransitions.find(WANDER_TRANSITION);
						if (iter != mTransitions.end())//found?
						{
							return &(iter->second);
						}
						std::cout << "ERROR - no matching transition!\n";
					}
				}
			}
		}
		else 
		{
			if (gpGame->getSystem()->getInputSystem()->isKeyPressed(InputSystem::THREE_KEY))
			{
				//If we are close to the player, as defined in data.txt, transition to the chase state.
				map<TransitionType, StateTransition>::iterator iter = mTransitions.find(WANDER_TRANSITION);
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

void HealState::scale()
{
	if (gpGame->getSystem()->getInputSystem()->isKeyPressed(InputSystem::FOUR_KEY))
		mScale += 1.0f;
	else if (gpGame->getSystem()->getInputSystem()->isKeyPressed(InputSystem::FIVE_KEY))
		mScale -= 1.0f;
	//cout << mScale << endl;
}
