#ifndef TILE_HPP
	#define TILE_HPP

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <vector>

namespace iq
{
	class tile;
	typedef boost::shared_ptr<tile> tile_ptr;
	typedef std::vector<tile> tile_vector;
}

	#include <types.hpp>

namespace iq
{
	class tile
	{
		protected:
		public:
			iq::BITMAP_ptr bitmap;

			tile(const iq::BITMAP_ptr);
	};
}

#endif

