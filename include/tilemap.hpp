
#ifndef IQ_TILEMAP_HPP
	#define IQ_TILEMAP_HPP

namespace iq
{
	class tilemap;
}

	#include <boost/shared_ptr.hpp>
	#include <string>
	#include <tile.hpp>
	#include <vector>

namespace iq
{
	class tilemap
	{
	private:
	protected:
	public:
		std::vector< std::vector<iq::tile> > tiles;

		tilemap(const std::string &);

		void load(const std::string &);
	};
}

#endif

