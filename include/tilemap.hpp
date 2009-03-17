
#ifndef IQ_TILEMAP_HPP
	#define IQ_TILEMAP_HPP

namespace iq
{
	class tilemap;
}

	#include <boost/shared_ptr.hpp>
	#include <stdexcept>
	#include <string>
	#include <tile.hpp>
	#include <vector>

namespace iq
{
	typedef boost::shared_ptr<tilemap> tilemap_ptr;

	class tilemap
	{
	private:
	protected:
			
		unsigned int MapTextureWidth;
		unsigned int MapTextureHeight;
		unsigned int MapNumTilesHigh;
		unsigned int MapNumTilesWide;
		unsigned int TileSize;			//in pixels (duh)
		unsigned int numLayers;
		unsigned int MapWidth;			//in tiles
		unsigned int MapHeight; 		//in tiles
		std::vector< std::vector< std::vector<tile_ptr > > > TileLayers;
		std::vector<tile_ptr> TileSet;
		BITMAP_ptr rawTileSetImage;
		void loadMapTexture(const TiXmlElement *MapTextureElement);
		void loadMapDimensions(const TiXmlElement *MapTextureElement);

	public:
		tilemap(const std::string &);

		const unsigned int h(void) const;
		void load(const std::string &){};
		void load(const TiXmlElement * const MapRoot);
		void loadHard();
		void draw(BITMAP_ptr scrbuf);
		
		const unsigned int w(void) const;
	};
}

#endif

