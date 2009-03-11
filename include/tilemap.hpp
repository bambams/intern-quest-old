
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
			std::string MapTextureFile;		//needs renaming
			boost::shared_ptr<BITMAP> MapTexture;
			unsigned int MapTextureWidth;
			unsigned int MapTextureHeight;
			unsigned int TileSize;			//in pixels (duh)
			unsigned int numLayers;
			unsigned int MapWidth;			//in tiles
			unsigned int MapHeight; 		//in tiles
			std::vector< std::vector< std::vector<tile> > > TileLayer;
	
	public:
		
		tilemap(const std::string &);

		const unsigned int h(void) const;
		void load(const std::string &);
		const unsigned int w(void) const;
	};
}

#endif

