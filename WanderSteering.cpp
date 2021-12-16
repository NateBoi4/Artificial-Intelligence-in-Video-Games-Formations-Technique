#include <cassert>

#include "Steering.h"
#include "SeekSteering.h"
#include "WanderSteering.h"
#include "Game.h"
#include "GraphicsSystem.h"
#include "UnitManager.h"
#include "Unit.h"


WanderSteering::WanderSteering(const UnitID& ownerID)
	: Steering(Steering::WANDER, ownerID)
{
}

Steering* WanderSteering::getSteering()
{
	//select point
	if (mTargetLoc == ZERO_VECTOR2D)
	{
		selectNewPoint();
	}
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	float diffSquared = diff.getLengthSquared();

	float newPointDistanceSquared = 10000.0f;

	if (diffSquared < newPointDistanceSquared)//time to choose a new target
	{
		selectNewPoint();
	}

	SeekSteering seek(mOwnerID, mTargetLoc);
	seek.update();
	const PhysicsData& seekData = seek.getData();

	data.acc = seekData.acc;
	data.vel = seekData.vel;
	this->mData = data;
	return this;
}

void WanderSteering::selectNewPoint()
{
	int x = rand() % gpGame->getGraphicsSystem()->getDisplayWidth();
	int y = rand() % gpGame->getGraphicsSystem()->getDisplayHeight();
	mTargetLoc = Vector2D(x, y);
}

