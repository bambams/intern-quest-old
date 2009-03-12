
#include <tile.hpp>

namespace iq
{
	tile::tile(iq::BITMAP_ptr img, bool Passability)
	{
		this->tileImage = img;
		this->isPassable = Passability;
	}

	void tile::passable(bool Passability)
	{
		this->isPassable = Passability;
	}
}

