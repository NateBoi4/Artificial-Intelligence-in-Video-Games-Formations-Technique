#include "GameUnit.h"
#include "Game.h"
#include "DataLoader.h"
#include "FontManager.h"
#include <DataRepository.h>
#include <GraphicsSystem.h>
#include <sstream>

void GameUnit::update(float elapsedTime)
{
	AutoFacingUnit::update(elapsedTime);
}

void GameUnit::draw() const
{
	if (gpGame->shouldDrawDebugData())
	{
		std::stringstream textStream;
		textStream << mHPs;

		Font* pFont = gpGame->getFontManager()->getFont(COUR_24_FONT_ID);
		//GraphicsSystem::writeText(getPosition(), *pFont, BLACK_COLOR, textStream.str(), Font::RIGHT);
	}

	Unit::draw();
}

void GameUnit::heal(int amt)
{
	mHPs += amt;
	if (mHPs > mMaxHPs)
		mHPs = mMaxHPs;
}

void GameUnit::damage(int amt)
{
	mHPs -= amt;
	if (mHPs < 0)
		mHPs = 0;
}

GameUnit::GameUnit(const Sprite& sprite)
	:AutoFacingUnit(sprite)
{
	mMaxHPs = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_HPS).getIntVal();
	mHPs = mMaxHPs;
}

GameUnit::~GameUnit()
{
}
