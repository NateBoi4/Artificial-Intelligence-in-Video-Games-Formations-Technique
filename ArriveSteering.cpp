#include <cassert>

#include "Steering.h"
#include "SeekSteering.h"
#include "ArriveSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


ArriveSteering::ArriveSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering(Steering::ARRIVE,ownerID,targetLoc,targetID)
{
}

Steering* ArriveSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//are we seeking a location or a unit?
	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	diff = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	float diffSquared = diff.getLengthSquared();

	float slowRadiusSquared = 10000.0f;
	float stopRadiusSquared = 100.0f;
	
	if (diffSquared > slowRadiusSquared)//full speed
	{
		//go as fast as you can
		SeekSteering seek(mOwnerID, mTargetLoc);
		seek.update();
		const PhysicsData& seekData = seek.getData();

		data.acc = seekData.acc;
	}
	else if (diffSquared <= stopRadiusSquared)//stop
	{
		data.acc = ZERO_VECTOR2D;
		data.vel = ZERO_VECTOR2D;
	}
	else //slow down
	{
		diff.normalize();
		diff *= pOwner->getMaxSpeed() * (diffSquared / slowRadiusSquared);
		data.acc = ZERO_VECTOR2D;
		data.vel = diff;
	}

	this->mData = data;
	return this;
}

