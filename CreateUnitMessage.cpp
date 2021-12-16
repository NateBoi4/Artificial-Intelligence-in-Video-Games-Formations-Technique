#include "Game.h"
#include "GameMessageManager.h"
#include "CreateUnitMessage.h"
#include "UnitManager.h"
#include "SpriteManager.h"

CreateUnitMessage::CreateUnitMessage()
	:GameMessage(CREATE_UNIT_MESSAGE)
{
}

CreateUnitMessage::~CreateUnitMessage()
{
}

void CreateUnitMessage::process()
{
	Unit* pUnit = gpGame->getUnitManager()->createRandomUnit(*gpGame->getSpriteManager()->getSprite(AI_ICON_SPRITE_ID));
}

