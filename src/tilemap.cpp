
#include <tilemap.hpp>

namespace iq
{
	tilemap::tilemap(const std::string &path)
	{
		this->loadHard();
	}
	
	void tilemap::loadMapTexture(const TiXmlElement * const MapTextureElement)
	{
		//BITMAP_ptr rawTileSet;
		// std::string rawTileSetPath;
		
		// const TiXmlElement *element;
		// if(element = MapTextureElement->FirstChild("FileName"))
			// throw std::runtime_error("MapXML is missing MAP_TEXTURE element");
		
		// rawTileSetFile = element->Value();
		
		// if(element = MapTextureElement->NextSiblingElement("Width"))
			// throw std::runtime_error("MapXML is missing MAP_TEXTURE Width element");
		
		// this->Width = element->Value;
		
		// if(element = MapTextureElement->NextSiblingElement("Height"))
			// throw std::runtime_error("MapXML is missing MAP_TEXTURE Height element");
			
		// this->Height = element->Value;
		
		// this->rawTileSetImage.reset(load_bitmap(rawTileSetFile.c_string()), destroy_bitmap);
	}
	
	void tilemap::loadMapDimensions(const TiXmlElement * const MapTextureElement)
	{
		// const TiXmlElement *element;
		// if(element = MapTextureElement->FirstChild("TileSize"))
			// throw std::runtime_error("MapXML is missing MAP_DIMENSIONS TileSize element");
		
		// this->TileSize = element->Value;
		
		// if(element = MapTextureElement->NextSiblingElement("Width"))
			// throw std::runtime_error("MapXML is missing MAP_TEXTURE Width element");
		
		// this->Width = element->Value
		
		// if(element = MapTextureElement->NextSiblingElement("Height"))
			// throw std::runtime_error("MapXML is missing MAP_TEXTURE Height element");
			
		// this->Height = element->Value
		
		// this->rawTileSetImage.reset(load_bitmap(rawTileSetFile.c_string()), destroy_bitmap);
	}
	void tilemap::load(const TiXmlElement * const MapRoot)
	{
	
		// const TiXmlElement *element = NULL;
	    // const char *str = NULL;

		// if((element = MapRoot->FirstChildElement("MAP_TEXTURE"))
			// throw std::runtime_error("MapXML is missing MAP_TEXTURE element");
		
		
		// this->loadMapTexture(element);
		// this->loadMapDimensions(element);
		
	}
	
	// WARNING! A DEADLY KLUDGE LURKS NEARBY!
	void tilemap::loadHard()
	{
		int n = 0;
		this->rawTileSetImage.reset(load_bitmap("media/tileset.bmp", NULL),destroy_bitmap);
		
		
		BITMAP_ptr tmpBitmap;
		tile_ptr tmpTile;
		
		this->MapTextureWidth = 64;
		this->MapTextureHeight = 96;
		this->MapNumTilesWide = 2;
		this->MapNumTilesHigh = 3;
		this->TileSize = 32;
		this->numLayers = 1;
		this->MapWidth = 10;
		this->MapHeight = 10;
		
		for(uint y = 0; y < MapNumTilesHigh; y++)
		{
			for(uint x = 0; x < MapNumTilesWide; x++) 
			{
				tmpBitmap.reset(create_bitmap(TileSize, TileSize), destroy_bitmap);
				
				blit(rawTileSetImage.get(), tmpBitmap.get(), x * TileSize, y * TileSize, 0, 0, TileSize, TileSize);
				
				tmpTile.reset(new tile(tmpBitmap));	
				
				TileSet.push_back(tmpTile);				
				
			}
		}
		
		int foo[][10][10] = {
			{
			{2,2,2,2,2,2,2,2,2,2},
			{4,4,4,4,4,4,4,4,4,4},
			{6,6,6,6,6,6,6,6,6,6},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
			{1,1,1,1,1,1,1,1,1,1},
				}
			};
		
		//STRAIGHT
		for(uint i = 0; i < numLayers; i++)
		{
			TileLayers.push_back(std::vector<std::vector<tile_ptr> >() );
			for(uint y = 0; y < MapHeight; y++)
			{
				TileLayers[i].push_back(std::vector<tile_ptr>());
				for(uint x = 0; x < MapWidth; x++)
				{
					this->TileLayers[i][y].push_back(TileSet[foo[i][y][x]-1]);
				}
			}
		}	
	}
	
	void tilemap::draw(BITMAP_ptr scrbuf)
	{
		for(uint l = 0, ll=this->TileLayers.size(); l < ll; l++)
		{
			for(uint y = 0, yy=this->TileLayers[l].size(); y < yy; y++)
			{
				for(uint x = 0, xx=this->TileLayers[l][y].size(); x < xx; x++)
				masked_blit(TileLayers[l][y][x]->tileImage.get(),scrbuf.get(),0,0,  x * this->TileSize, y*this->TileSize,TileLayers[l][y][x]->tileImage->w,TileLayers[l][y][x]->tileImage->h);
			}
		}
	}
}

