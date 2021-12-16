#define NOMINMAX
#include <stdio.h>

#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <crtdbg.h>
#include <EventSystem.h>
#include <Vector2D.h>
#include <algorithm>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "Sprite.h"
#include "Unit.h"
#include "Timer.h"
#include "PerformanceTracker.h"
#include "MemoryTracker.h"
#include "NotificationQueue.h"
#include "CircularQueue.h"
#include "ComponentManager.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"

using namespace std;

PerformanceTracker* gpPerformanceTracker = NULL;

int main(int argc, char **argv)
{
	EventSystem::initInstance();

	gpPerformanceTracker = new PerformanceTracker;
	gpPerformanceTracker->startTracking("init");

	//create the global game object
	gpGame = new Game;
	//init the game
	bool goodGame = gpGame->init();
	if (!goodGame)
	{
		fprintf(stderr, "failed to initialize Game object!\n");
		return -1;
	}

	gpPerformanceTracker->stopTracking("init");
	cout << "initialization took:" << gpPerformanceTracker->getElapsedTime("init") << "ms\n";

	gpGame->doLoop();

	gpGame->cleanup();
	delete gpGame;
	gpGame = NULL;

	delete gpPerformanceTracker;

	EventSystem::cleanupInstance();

	MemoryTracker::getInstance()->reportAllocations( cout );
	system("pause");

	return 0;
}