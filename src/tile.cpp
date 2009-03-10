
#include <tile.hpp>

namespace iq
{
	tile::tile(boost::shared_ptr<BITMAP> bitmap)
	{
		this->m_bitmap = bitmap;
	}

	const boost::shared_ptr<BITMAP> tile::bitmap(void) const
	{
		return(this->m_bitmap);
	}
}

