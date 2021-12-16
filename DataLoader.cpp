#include "DataLoader.h"
#include "DataRepository.h"
#include <fstream>
#include <sstream>
#include <windows.h>

using namespace std;

DataLoader::DataLoader(const string& filename, DataRepository* pRepository)
{
	ifstream input( filename );
	if( input.good() )
	{
		while( !input.eof() )
		{
			string line;
			getline( input, line);
			stringstream sstream( line );
			string id;
			sstream >> id;

			if( id == "screen_size" )
			{
				UINT width, height;
				sstream >> width >> height;
				pRepository->addEntry( DataKeyEnum::SCREEN_WIDTH, width );
				pRepository->addEntry( DataKeyEnum::SCREEN_HEIGHT, height );
			}
			else if (id == "target_fps")
			{
				UINT fps;
				sstream >> fps;
				pRepository->addEntry(DataKeyEnum::TARGET_FPS, fps);
			}
			else if (id == "max_units")
			{
				UINT max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_UNITS, max);
			}
			else if (id == "unit_max_acc")
			{
				float max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_ACC, max);
			}
			else if (id == "unit_max_speed")
			{
				float max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_SPEED, max);
			}
			else if (id == "unit_max_rot_acc")
			{
				float max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_ROT_ACC, max);
			}
			else if (id == "unit_max_rot_vel")
			{
				float max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_ROT_VEL, max);
			}
			else if (id == "max_hps")
			{
				int max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_HPS, max);
			}
			else if (id == "heal_range")
			{
				UINT val;
				sstream >> val;
				pRepository->addEntry(DataKeyEnum::MAX_HEAL_RANGE_SQUARED, val*val);
			}
			else if (id == "damage_range")
			{
				UINT val;
				sstream >> val;
				pRepository->addEntry(DataKeyEnum::MAX_DAMAGE_RANGE_SQUARED, val*val);
			}
			else if (id == "heal_amt")
			{
				int amt;
				sstream >> amt;
				pRepository->addEntry(DataKeyEnum::HEAL_AMT, amt);
			}
			else if (id == "damage_amt")
			{
				int amt;
				sstream >> amt;
				pRepository->addEntry(DataKeyEnum::DAMAGE_AMT, amt);
			}
			else if (id == "close")
			{
				int amt;
				sstream >> amt;
				pRepository->addEntry(DataKeyEnum::CLOSE_DIST, amt * amt);
			}
			else if (id == "hurt")
			{
				int amt;
				sstream >> amt;
				pRepository->addEntry(DataKeyEnum::HURT_AMT, amt);
			}
			else if (id == "healed")
			{
				int amt;
				sstream >> amt;
				pRepository->addEntry(DataKeyEnum::HEALED_AMT, amt);
			}
			else
			{
				cout << "Unknown tag:" << id << endl;
			}
		}
	}

}


