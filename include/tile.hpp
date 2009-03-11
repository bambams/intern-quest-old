#ifndef TILE_HPP
#define TILE_HPP

namespace iq
{
	class tile;
}

#include <boost/shared_ptr.hpp>
#include <allegro.h>

namespace iq
{
	class tile
	{
		protected:
			boost::shared_ptr<BITMAP> tileImage;
			bool isPassable;
		public:
			tile(boost::shared_ptr<BITMAP> image, bool Passability);
			void passable(bool Passability);
			bool passable(){ return isPassable;}
	};
}
#endif
