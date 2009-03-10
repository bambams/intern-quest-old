
#ifndef IQ_TILE_HPP
	#define IQ_TILE_HPP

namespace iq
{
	class tile;
}

	#include <animation.hpp>
	#include <boost/shared_ptr.hpp>

namespace iq
{
	class tile
	{
	private:
	protected:
		boost::shared_ptr<BITMAP> m_bitmap;
	public:
		tile(boost::shared_ptr<BITMAP>);

		const boost::shared_ptr<BITMAP> bitmap(void) const;
	};
}

#endif

