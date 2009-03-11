
#include <tile.hpp>

namespace iq
{
	tile::tile(boost::shared_ptr<BITMAP> img, bool Passability)
	{
		this->tileImage = img;
		this->isPassable = Passability;
	}

	void tile::passable(bool Passability)
	{
		this->isPassable = Passability;
	}
}

