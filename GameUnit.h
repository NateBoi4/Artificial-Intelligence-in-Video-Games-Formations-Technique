#pragma once

#pragma once

#include "AutoFacingUnit.h"

class GameUnit :public AutoFacingUnit
{
public:
	void update(float elapsedTime);
	void draw() const;

	void heal(int amt);
	void damage(int amt);

	int getHP() { return mHPs; };

protected:
	int mHPs=0;
	int mMaxHPs=0;

	GameUnit(const Sprite& sprite);
	virtual ~GameUnit();

	GameUnit(GameUnit&);//invalidate copy constructor
	void operator=(GameUnit&);//invalidate assignment operator

	friend class UnitManager;

};
