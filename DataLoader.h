#pragma once
#include <Trackable.h>
#include <DataRepository.h>
#include <string>

class DataLoader : public Trackable
{
public:
	DataLoader( const std::string& filename, DataRepository* pDepository );
	~DataLoader(){};
private:

};

//class only exists to hold a class level enum
class DataKeyEnum
{
public:
	enum DataKeyVals
	{
		TARGET_FPS,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		MAX_UNITS,
		MAX_ACC,
		MAX_SPEED,
		MAX_ROT_ACC,
		MAX_ROT_VEL,
		MAX_HPS,
		MAX_HEAL_RANGE_SQUARED,
		MAX_DAMAGE_RANGE_SQUARED,
		HEAL_AMT,
		DAMAGE_AMT,
		CLOSE_DIST,
		HURT_AMT,
		HEALED_AMT
	};
};
