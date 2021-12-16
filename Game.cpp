#include <stdio.h>
#include <assert.h>
#include <iostream>

#include <sstream>

#include "Game.h"
#include <System.h>
#include <GraphicsSystem.h>
#include <GraphicsBuffer.h>
#include <Font.h>
#include <FontManager.h>
#include <GraphicsBufferManager.h>
#include <InputSystem.h>
#include "GameMessageManager.h"
#include <Sprite.h>
#include "SpriteManager.h"
#include <Timer.h>
#include <DataRepository.h>
#include "PlayerMoveToMessage.h"
#include "CreateUnitMessage.h"
#include "ComponentManager.h"
#include "UnitManager.h"
#include "DataLoader.h"
#include "ExtraColors.h"
#include "StateMachine.h"
#include "WanderState.h"
#include "HealState.h"
#include "ChaseState.h"

Game* gpGame = NULL;

Game::Game()
	:Trackable("game class")
	,mpSystem(NULL)
	,mpGraphicsBufferManager(NULL)
	,mpSpriteManager(NULL)
	,mpFontManager(NULL)
	,mpLoopTimer(NULL)
	,mpMasterTimer(NULL)
	,mpFont(NULL)
	,mShouldExit(false)
	,mBackgroundBufferID("")
	,mpMessageManager(NULL)
	,mpComponentManager(NULL)
	,mpUnitManager(NULL)
	,mpRepository(NULL)
	,mTimeLastFrame(0.0f)
{
}

Game::~Game()
{
	cleanup();
}

bool Game::init()
{
	mShouldExit = false;

	//create Timers
	mpLoopTimer = new Timer;
	mpMasterTimer = new Timer;

	mpRepository = new DataRepository;
	DataLoader loader("data.txt", mpRepository);

	//create and init GraphicsSystem
	mpSystem = new System();
	bool goodGraphics = mpSystem->init( mpRepository->getEntry(DataKeyEnum::SCREEN_WIDTH).getUIntVal(), mpRepository->getEntry(DataKeyEnum::SCREEN_HEIGHT).getUIntVal());
	if(!goodGraphics) 
	{
		fprintf(stderr, "failed to initialize GraphicsSystem object!\n");
		return false;
	}

	mpGraphicsBufferManager = new GraphicsBufferManager(mpSystem->getGraphicsSystem());
	mpSpriteManager = new SpriteManager;
	mpFontManager = new FontManager;

	mpMessageManager = new GameMessageManager();

	UINT maxUnits = mpRepository->getEntry(DataKeyEnum::MAX_UNITS).getUIntVal();
	mpComponentManager = new ComponentManager(maxUnits);
	mpUnitManager = new UnitManager(maxUnits);

	//load buffers
	GraphicsBuffer* pBuff;
	pBuff = mpGraphicsBufferManager->loadBuffer(mBackgroundBufferID,"wallpaper.bmp");
	assert(pBuff);
	pBuff = mpGraphicsBufferManager->loadBuffer(mPlayerIconBufferID,"arrow.png");
	assert(pBuff);
	pBuff = mpGraphicsBufferManager->loadBuffer(mEnemyIconBufferID, "enemy-arrow.png");
	assert(pBuff);
	pBuff = mpGraphicsBufferManager->loadBuffer(mEnemyChaseIconBufferID, "enemy-arrow-chase.png");
	assert(pBuff);
	pBuff = mpGraphicsBufferManager->loadBuffer(mEnemyHealIconBufferID, "enemy-arrow-heal.png");
	assert(pBuff);
	pBuff = mpGraphicsBufferManager->loadBuffer(mHealIconBufferID, "heal.png");
	assert(pBuff);
	pBuff = mpGraphicsBufferManager->loadBuffer(mTargetBufferID,"target.png");
	assert(pBuff);

	//load Font
	mpFont = mpFontManager->createAndManageFont(COUR_24_FONT_ID, "cour.ttf", 24);
	
	//setup sprites
	GraphicsBuffer* pBackGroundBuffer = mpGraphicsBufferManager->getBuffer( mBackgroundBufferID );
	if( pBackGroundBuffer != NULL )
	{

		mpSpriteManager->createAndManageSprite( BACKGROUND_SPRITE_ID, pBackGroundBuffer, 0, 0, (float)pBackGroundBuffer->getWidth(), (float)pBackGroundBuffer->getHeight() );
	}
	GraphicsBuffer* pPlayerBuffer = mpGraphicsBufferManager->getBuffer( mPlayerIconBufferID );
	Sprite* pArrowSprite = NULL;
	if( pPlayerBuffer != NULL )
	{
		pArrowSprite = mpSpriteManager->createAndManageSprite( PLAYER_ICON_SPRITE_ID, pPlayerBuffer, 0, 0, (float)pPlayerBuffer->getWidth(), (float)pPlayerBuffer->getHeight() );
	}

	GraphicsBuffer* pAIBuffer = mpGraphicsBufferManager->getBuffer(mEnemyIconBufferID);
	Sprite* pEnemyArrow = NULL;
	if (pAIBuffer != NULL)
	{
		pEnemyArrow = mpSpriteManager->createAndManageSprite(AI_ICON_SPRITE_ID, pAIBuffer, 0, 0, (float)pAIBuffer->getWidth(), (float)pAIBuffer->getHeight());
	}

	GraphicsBuffer* pAIChaseBuffer = mpGraphicsBufferManager->getBuffer(mEnemyChaseIconBufferID);
	if (pAIChaseBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(AI_CHASE_ICON_SPRITE_ID, pAIChaseBuffer, 0, 0, (float)pAIChaseBuffer->getWidth(), (float)pAIChaseBuffer->getHeight());
	}

	GraphicsBuffer* pAIHealBuffer = mpGraphicsBufferManager->getBuffer(mEnemyHealIconBufferID);
	if (pAIHealBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(AI_HEAL_ICON_SPRITE_ID, pAIHealBuffer, 0, 0, (float)pAIHealBuffer->getWidth(), (float)pAIHealBuffer->getHeight());
	}

	GraphicsBuffer* pHealBuffer = mpGraphicsBufferManager->getBuffer(mHealIconBufferID);
	if (pAIHealBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(HEAL_ICON_SPRITE_ID, pHealBuffer, 0, 0, (float)pHealBuffer->getWidth(), (float)pHealBuffer->getHeight());
	}

	GraphicsBuffer* pTargetBuffer = mpGraphicsBufferManager->getBuffer(mTargetBufferID);
	if (pTargetBuffer != NULL)
	{
		mpSpriteManager->createAndManageSprite(TARGET_SPRITE_ID, pTargetBuffer, 0, 0, (float)pTargetBuffer->getWidth(), (float)pTargetBuffer->getHeight());
	}

	
	//setup units
	Unit* pUnit = NULL;
	
	if (pArrowSprite)
	{
		Vector2D center((int)getGraphicsSystem()->getDisplayWidth() / 2, (int)getGraphicsSystem()->getDisplayHeight() / 2);
		//pUnit = mpUnitManager->createPlayerUnit(*pArrowSprite, false, PositionData(center, 0.0f));
		//pUnit->setShowTarget(true);
		//pUnit->setSteering(Steering::SEEK, Vector2D((int)getGraphicsSystem()->getDisplayWidth() / 2, 600));
	}
	

	//pUnit = mpUnitManager->createUnit(*mpSpriteManager->getSprite(HEAL_ICON_SPRITE_ID), false, PositionData(),PhysicsData(), HEAL_UNIT_ID);

	mTargetFPS = mpRepository->getEntry(DataKeyEnum::TARGET_FPS).getUIntVal();
	mTargetElapsedTime = 1000.0f / mTargetFPS;

	return true;
}

void Game::cleanup()
{
	//delete the timers
	delete mpLoopTimer;
	mpLoopTimer = NULL;
	delete mpMasterTimer;
	mpMasterTimer = NULL;

	delete mpFontManager;
	mpFontManager = NULL;

	delete mpRepository;
	mpRepository = NULL;

	mpFont = NULL;

	//delete the graphics system
	delete mpSystem;
	mpSystem = NULL;

	delete mpGraphicsBufferManager;
	mpGraphicsBufferManager = NULL;
	delete mpSpriteManager;
	mpSpriteManager = NULL;
	delete mpMessageManager;
	mpMessageManager = NULL;
	delete mpUnitManager;
	mpUnitManager = NULL;
	delete mpComponentManager;
	mpComponentManager = NULL;
}

void Game::doLoop()
{
	//game loop
	while (!mShouldExit)
	{
		gpPerformanceTracker->clearTracker("loop");
		gpPerformanceTracker->startTracking("loop");

		gpGame->beginLoop();

		gpPerformanceTracker->clearTracker("process");
		gpPerformanceTracker->startTracking("process");

		gpGame->processLoop();

		gpPerformanceTracker->stopTracking("process");

		gpGame->endLoop();

		gpPerformanceTracker->stopTracking("loop");
		//std::cout << "loop took:" << gpPerformanceTracker->getElapsedTime("loop") << "ms    ";
		//std::cout << "processing took:" << gpPerformanceTracker->getElapsedTime("process") << "ms\n";
		mTimeLastFrame = (float)gpPerformanceTracker->getElapsedTime("loop");
	}
}

void Game::beginLoop()
{
	mpLoopTimer->start();
}

void Game::processLoop()
{
	InputSystem* pInputSystem = mpSystem->getInputSystem();

	float dt = (mTargetElapsedTime * mTimeMult) / 1000.0f;
	mpUnitManager->updateAll(dt);
	mpComponentManager->update(dt);
	pInputSystem->update(dt);
	
	//potentially move the heal loc
	updateHealLoc();

	Sprite* pBackgroundSprite = mpSpriteManager->getSprite(BACKGROUND_SPRITE_ID);
	GraphicsSystem::draw(Vector2D(0, 0), *pBackgroundSprite);

	//draw units
	mpUnitManager->drawAll();

	if (mDrawDebugData)
	{
		drawDebugData();
	}

	mpSystem->getGraphicsSystem()->flip();

	Vector2D pos = pInputSystem->getCurrentMousePos();

	mpMessageManager->processMessagesForThisframe();

	if(pInputSystem->isMouseButtonPressed(InputSystem::LEFT))
	{
		GameMessage* pMessage = new PlayerMoveToMessage( pos );
		MESSAGE_MANAGER->addMessage( pMessage, 0 );
	}

	if(pInputSystem->isKeyPressed(InputSystem::ESCAPE_KEY))
	{
		mShouldExit = true;
	}

	if (pInputSystem->isKeyPressed(InputSystem::M_KEY))
	{
		for (int i = 0; i < mpRepository->getEntry(DataKeyEnum::MAX_UNITS).getUIntVal(); i++)
		{
			GameMessage* pMessage = new CreateUnitMessage;
			MESSAGE_MANAGER->addMessage(pMessage, 100);
		}
	}

	if (pInputSystem->isKeyPressed(InputSystem::UP_KEY))
	{
		GameMessage* pMessage = new CreateUnitMessage;
		MESSAGE_MANAGER->addMessage(pMessage);
	}
	else if (pInputSystem->isKeyPressed(InputSystem::DOWN_KEY))
	{
		mpUnitManager->deleteRandomUnit();
	}

	if (pInputSystem->isKeyPressed(InputSystem::LEFT_KEY))
	{
		mTimeMult -= 0.1f;
		if (mTimeMult < 0.0f)
			mTimeMult = 0.0f;
	}
	else if (pInputSystem->isKeyPressed(InputSystem::RIGHT_KEY))
	{
		mTimeMult += 0.1f;
	}
	else if (pInputSystem->isKeyPressed(InputSystem::SPACE_KEY))
	{
		mTimeMult = 1.0f;
	}


	if (pInputSystem->isKeyPressed(InputSystem::D_KEY))
	{
		mDrawDebugData = false;
	}
	else
	{
		mDrawDebugData = true;
	}
}

void Game::endLoop()
{
	mpUnitManager->cleanupDeadUnits();
	//mpMasterTimer->start();
	mpLoopTimer->sleepUntilElapsed( mTargetElapsedTime );
}

void Game::drawDebugData()
{
	//InputSystem* pInputSystem = mpSystem->getInputSystem();

	//Vector2D pos = pInputSystem->getCurrentMousePos();

	//create mouse text
	std::stringstream textStream;
	//textStream << pos;

	//write text at mouse position
	//GraphicsSystem::writeText(pos, *mpFont, BLACK_COLOR, textStream.str(), Font::RIGHT);

	textStream.str("");
	textStream.clear();

	//write out current number of Units
	//Uint32 numUnits = mpUnitManager->getNumUnits();
	textStream << "Cycle through scalable formations with 1, 2, and 3."; ///<< numUnits;
	GraphicsSystem::writeText(Vector2D(GraphicsSystem::getDisplayWidth() / 2, 0), *mpFont, BLACK_COLOR, textStream.str(), Font::CENTER);

	textStream.str("");
	textStream.clear();

	//write out current fps
	//int fps = (int)((1000.0f / mTimeLastFrame) + .5f);//+.5f does rounding
	textStream << "Scale Formations with 4 and 5.";
	GraphicsSystem::writeText(Vector2D(GraphicsSystem::getDisplayWidth() / 2, 20), *mpFont, BLACK_COLOR, textStream.str(), Font::CENTER);

	textStream.str("");
	textStream.clear();

	//write out time multiple
	textStream << "Spawn units with M. Can be used twice."; //<< mTimeMult;
	GraphicsSystem::writeText(Vector2D(GraphicsSystem::getDisplayWidth() / 2, 40), *mpFont, BLACK_COLOR, textStream.str(), Font::CENTER);

}

void Game::updateHealLoc()
{
	if (rand() % 100 == 99)
	{
		Vector2D loc = Vector2D((float)(rand() % GraphicsSystem::getDisplayWidth()), (float)(rand() % GraphicsSystem::getDisplayHeight()));
		Unit* pUnit = mpUnitManager->getUnit(HEAL_UNIT_ID);
		if (pUnit)
		{
			pUnit->getPositionComponent()->setPosition(loc);
		}
	}
}

GraphicsSystem* Game::getGraphicsSystem() const
{
	return mpSystem->getGraphicsSystem();
}

float genRandomBinomial()
{
	return genRandomFloat() - genRandomFloat();
}

float genRandomFloat()
{
	float r = (float)rand()/(float)RAND_MAX;
	return r;
}

