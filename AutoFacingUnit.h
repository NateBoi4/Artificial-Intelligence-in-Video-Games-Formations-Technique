#pragma once

#include "Unit.h"
#include "UnitManager.h"

class AutoFacingUnit :public Unit
{
public:
	virtual void update(float elapsedTime);

protected:
	AutoFacingUnit(const Sprite& sprite);
	virtual ~AutoFacingUnit();

	AutoFacingUnit(AutoFacingUnit&);//invalidate copy constructor
	void operator=(AutoFacingUnit&);//invalidate assignment operator

	friend class UnitManager;

};