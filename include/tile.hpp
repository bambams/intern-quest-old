#ifndef TILE_HPP
	#define TILE_HPP

namespace iq
{
	class tile;
}

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <types.hpp>
	#include <vector>

namespace iq
{
	typedef boost::shared_ptr<tile> tile_ptr;
	typedef std::vector<tile> tile_vector;

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
