
#include <tilemap.hpp>

namespace iq
{
	tilemap::tilemap(const std::string &path, std::map<std::string, iq::tile_ptr> &tiles)
	{
		this->load(path, tiles);
	}

	void tilemap::draw(const iq::BITMAP_ptr scrbuf, const iq::entity_ptr player) const
	{
//static bool flag = true;
		int map_x, map_y;
		iq::BITMAP_ptr bitmap;

		for(iq::uint z=0, zlen=this->layers.size(); z<zlen; z++)
		{
			map_y = -(player->y()-(scrbuf->h/2.0));

			for(int y=0, ylen=this->layers[z].size(); y<ylen; y++)
			{
				map_x = -(player->x()-(scrbuf->w/2.0));

				for(int x=0, xlen=this->layers[z][y].size(); x<xlen; x++)
				{
					bitmap = this->layers[z][y][x]->bitmap;
//if(flag)
//printf("Container: x,y,z=%3d,%3d,%3d\tScreen: x,y,z=%3d,%3d,%3d\n", x, y, z, map_x + (x * bitmap->w), map_y + (y * bitmap->h), z);

					masked_blit(bitmap.get(), scrbuf.get(), 0, 0, map_x + (x*this->tilesize), map_y + (y*this->tilesize), bitmap->w, bitmap->h);
				}
			}
		}
//flag=false;
	}

	const std::vector<tilemap::tilelayer> tilemap::get_layers(void) const
	{
		return this->layers;
	}

	void tilemap::load(const std::string &path, std::map<std::string, iq::tile_ptr> &tiles)
	{
		iq::TiXmlDocument_ptr doc(new TiXmlDocument(path));
		TiXmlElement *root = NULL;

		if(!doc->LoadFile())
			throw std::runtime_error("Failed to load tilemap from XML document '" + path + "'.");

		if((root = doc->RootElement()) == NULL)
			throw std::runtime_error("Tilemap XML missing root element.");

		this->load(root, path, tiles);
	}

	void tilemap::load(const TiXmlElement * const map, const std::string &path, std::map<std::string, iq::tile_ptr> &tiles)
	{
		const TiXmlElement *element = NULL;
		const TiXmlElement *texture = NULL;
		const TiXmlElement *dimensions = NULL;
		const TiXmlElement *layout = NULL;
		iq::uint num_layers, w, h;
		std::string tag_name;
		iq::BITMAP_ptr tileset;

		if((element = map->FirstChildElement()) == NULL)
			throw std::runtime_error("Tilemap XML root element has no element children.");

		do
		{
			tag_name = element->ValueStr();

			if(tag_name == "MAP_TEXTURE")
				texture = element;
			else if(tag_name == "MAP_DIMENSIONS")
				dimensions = element;
			else if(tag_name == "LAYOUT")
				layout = element;
		}while((element = element->NextSiblingElement()));

		if(texture == NULL)
			throw std::runtime_error("Tilemap XML is missing MAP_TEXTURE element.");

		if(dimensions == NULL)
			throw std::runtime_error("Tilemap XML is missing MAP_DIMENSIONS element.");

		if(layout == NULL)
			throw std::runtime_error("Tilemap XML is missing LAYOUT element.");

		tileset = this->load_tileset(texture);
		this->load_dimensions(dimensions, num_layers, w, h);
		this->load_layout(layout, path, tileset, num_layers, w, h, tiles);
	}

	void tilemap::load_dimensions(const TiXmlElement * const dimensions, iq::uint &num_layers, iq::uint &w, iq::uint &h)
	{
		const TiXmlElement *element = NULL;
		const TiXmlNode *node = NULL;
		std::string s;
		std::stringstream ss;

		if((element = dimensions->FirstChildElement("Tile_Size")) == NULL)
			throw std::runtime_error("MAP_DIMENSIONS XML is missing the Tile_Size element.");

		if((node = element->FirstChild()) == NULL || node->Type() != TiXmlNode::TEXT)
			throw std::runtime_error("Tile_Size XML is invalid.");

		ss.clear();
		ss.str(s = node->ToText()->ValueStr());

		if(!(ss >> this->tilesize))
			throw std::runtime_error("Invalid tilesize '" + s + "'.");

		if((element = element->NextSiblingElement("Layers")) == NULL)
			throw std::runtime_error("MAP_DIMENSIONS XML is missing Layers element.");

		if((node = element->FirstChild()) == NULL || node->Type() != TiXmlNode::TEXT)
			throw std::runtime_error("Layers XML element is invalid.");

		ss.clear();
		ss.str(s = node->ToText()->ValueStr());

		if(!(ss >> num_layers))
			throw std::runtime_error("Invalid tilemap layers count '" + s + "'.");

		if((element = element->NextSiblingElement("Width")) == NULL)
			throw std::runtime_error("MAP_DIMENSIONS XML is missing Width element.");

		if((node = element->FirstChild()) == NULL || node->Type() != TiXmlElement::TEXT)
			throw std::runtime_error("Width XML element is invalid.");

		ss.clear();
		ss.str(s = node->ToText()->ValueStr());

		if(!(ss >> w))
			throw std::runtime_error("Invalid tilemap width '" + s + "'.");

		if((element = element->NextSiblingElement("Height")) == NULL)
			throw std::runtime_error("MAP_DIMENSIONS XML is missing Height element.");

		if((node = element->FirstChild()) == NULL || node->Type() != TiXmlElement::TEXT)
			throw std::runtime_error("Height XML element is invalid.");

		ss.clear();
		ss.str(s = node->ToText()->ValueStr());

		if(!(ss >> h))
			throw std::runtime_error("Invalid tilemap height '" + s + "'.");

//printf("tilesize=%d layers=%d width=%d height=%d\n", this->tilesize, num_layers, w, h);
	}

	const std::vector< std::vector<iq::tile_ptr> > tilemap::load_layer(const TiXmlElement * const layer, const std::string &path, const iq::BITMAP_ptr tileset, const iq::uint &w, const iq::uint &h, std::map<std::string, iq::tile_ptr> &tiles)
	{
		const TiXmlElement *element = NULL;
		std::vector< std::vector<iq::tile_ptr> > l;
		std::string tag_name;

		if((element = layer->FirstChildElement()) == NULL)
			throw std::runtime_error("Layer XML missing one or more RowInfo elements.");

		do
		{
			tag_name = element->ValueStr();

			if(tag_name == "RowInfo")
				l.push_back(this->load_rowinfo(element, path, tileset, w, tiles));
		}while((element = element->NextSiblingElement()));

		if(l.size() != h)
		{
			throw std::runtime_error(
				"Layer XML row count '"
				+ boost::lexical_cast<std::string>(l.size())
				+ "' does not match defined Height '"
				+ boost::lexical_cast<std::string>(h)
				+ "'.");
		}

		return l;
	}

	void tilemap::load_layout(const TiXmlElement * const layout, const std::string &path, const iq::BITMAP_ptr tileset, const iq::uint &num_layers, const iq::uint &w, const iq::uint &h, std::map<std::string, iq::tile_ptr> &tiles)
	{
		const TiXmlElement *element = NULL;
		std::string tag_name;

		if((element = layout->FirstChildElement()) == NULL)
			std::runtime_error("LAYOUT XML missing one or more Layer elements.");

		do
		{
			tag_name = element->ValueStr();

			if(tag_name == "Layer")
				this->layers.push_back(this->load_layer(element, path, tileset, w, h, tiles));
			else if(tag_name == "CollisionLayer")
				;
		}while((element = element->NextSiblingElement()));

		if(this->layers.size() != num_layers)
			throw std::runtime_error("Tilemap XML Layers count does not match the number of Layer elements.");
	}

	const std::vector<iq::tile_ptr> tilemap::load_rowinfo(const TiXmlElement * const rowinfo, const std::string &path, const BITMAP_ptr tileset, const iq::uint &w, std::map<std::string, iq::tile_ptr> &tiles)
	{
		long i;
		const TiXmlNode *node = NULL;
		std::stringstream ss;
		std::vector<iq::tile_ptr> tilerow;

		if((node = rowinfo->FirstChild()) == NULL || node->Type() != TiXmlNode::TEXT)
			throw std::runtime_error("RowInfo XML is invalid.");

		ss.clear();
		ss.str(node->ToText()->ValueStr());

		while(ss >> i)
		{
			if(i == -1)
				tilerow.push_back(this->void_tile(tiles));
			else
				tilerow.push_back(this->load_tile(tiles, tileset, path, (iq::uint)i));
			ss.ignore(1);
		}

		if(tilerow.size() != w)
		{
			throw std::runtime_error(
				"RowInfo XML tile count '"
				+ boost::lexical_cast<std::string>(tilerow.size())
				+ "' does not match defined Width '"
				+ boost::lexical_cast<std::string>(w)
				+ "'.");
		}

		return tilerow;
	}

	const tile_ptr tilemap::load_tile(std::map<std::string, iq::tile_ptr> &tiles, const iq::BITMAP_ptr tileset, const std::string &path, const iq::uint i)
	{
		iq::BITMAP_ptr bitmap;
		std::map<std::string, iq::tile_ptr>::iterator it;
		iq::uint j=1;
		std::string k = path + ":" + boost::lexical_cast<std::string>(i);
		iq::tile_ptr tile;
		iq::uint x, y;

		if((it = tiles.find(k)) != tiles.end())
			return it->second;
		
		bitmap.reset(create_bitmap(this->tilesize, this->tilesize), destroy_bitmap);

		if(bitmap.get() == NULL)
			throw std::runtime_error("Failed to allocate memory for tile bitmap.");

		for(y=0; y < (iq::uint)tileset->h; y += this->tilesize)
			for(x=0; x < (iq::uint)tileset->w; x += this->tilesize)
				if(j++ == i)
					goto createtile;

		throw std::runtime_error(
			"Invalid tile index '"
			+ boost::lexical_cast<std::string>(i)
			+ "' Valid indexes are 1-"
			+ boost::lexical_cast<std::string>(tileset->w * tileset->h)
			+ ".");

createtile:
		blit(tileset.get(), bitmap.get(), x, y, 0, 0, this->tilesize, this->tilesize);

		tile.reset(new iq::tile(bitmap));

		tiles[k] = tile;

		return tile;
	}

	const BITMAP_ptr tilemap::load_tileset(const TiXmlElement * const texture)
	{
		const TiXmlElement *element = NULL;
		const TiXmlNode *node = NULL;
		std::string path;
		iq::BITMAP_ptr tileset;

		if((element = texture->FirstChildElement("FileName")) == NULL)
			throw std::runtime_error("MAP_TEXTURE XML is missing FileName element.");

		if((node = element->FirstChild()) == NULL || node->Type() != TiXmlNode::TEXT)
			throw std::runtime_error("FileName XML is missing it's text value.");

		path = node->ToText()->ValueStr();

		tileset.reset(load_bmp(path.c_str(), NULL), destroy_bitmap);

		if(tileset.get() == NULL)
			throw std::runtime_error("Failed to load tileset '" + path + "'.");

		return tileset;
	}

	const iq::tile_ptr tilemap::void_tile(std::map<std::string, iq::tile_ptr> &tiles) const
	{
		iq::BITMAP_ptr bitmap;
		std::map<std::string, iq::tile_ptr>::iterator it;
		std::string k = "void:-1";
		iq::tile_ptr tile;

		if((it = tiles.find(k)) != tiles.end())
			return it->second;

		bitmap.reset(create_bitmap(this->tilesize, this->tilesize), destroy_bitmap);

		if(bitmap.get() == NULL)
			throw std::runtime_error("Failed to allocate void tile.");

		rectfill(bitmap.get(), 0, 0, this->tilesize, this->tilesize, makecol(255, 0, 255));

		tile.reset(new iq::tile(bitmap));

		tiles[k] = tile;

		return tile;
	}

	const iq::uint tilemap::get_tilesize(void) const
	{
		return this->tilesize;
	}
}

