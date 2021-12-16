#include "AutoFacingUnit.h"
#include <Vector2D.h>

AutoFacingUnit::AutoFacingUnit(const Sprite& sprite)
	:Unit(sprite)
{
}

AutoFacingUnit::~AutoFacingUnit()
{
}

void AutoFacingUnit::update(float elapsedTime)
{
	PositionComponent* pPosition = getPositionComponent();
	PhysicsComponent* pPhysics = getPhysicsComponent();

	const Vector2D& velocity = pPhysics->getVelocity();
	double newFacing = velocity.calcFacing();

	pPosition->setFacing((float)newFacing);

	//allow parent to update
	Unit::update(elapsedTime);
}

