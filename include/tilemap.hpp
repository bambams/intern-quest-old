
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
		unsigned int m_w, m_h;
	public:
		std::vector< std::vector<iq::tile> > tiles;

		tilemap(const std::string &);

		const unsigned int h(void) const;
		void load(const std::string &);
		const unsigned int w(void) const;
	};
}

#endif

