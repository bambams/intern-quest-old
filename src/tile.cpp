
#include <tile.hpp>

namespace iq
{
	tile::tile(boost::shared_ptr<BITMAP> img, bool Passability)
	{
		this->tileImage = img;
		this->isPassable = Passability;
	}

	const boost::shared_ptr<BITMAP> tile::tileImage(void) const
	{
		return(this->m_bitmap);
	}
}

