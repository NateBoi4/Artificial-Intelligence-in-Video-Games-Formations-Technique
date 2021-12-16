#pragma once

#include <Trackable.h>
#include "Steering.h"

class WanderSteering : public Steering
{
public:
	WanderSteering(const UnitID& ownerID);

protected:
	virtual Steering* getSteering();
	void selectNewPoint();
};