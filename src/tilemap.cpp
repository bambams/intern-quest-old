/*
 * Intern's Quest is an adventure game.
 * Copyright (C) 2009 Brandon McCaig, Samuel Henderson
 *
 * This file is part of Intern's Quest.
 *
 * Intern's Quest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Intern's Quest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Intern's Quest.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <tilemap.hpp>

namespace iq
{
	tilemap::tilemap(const std::string &path, std::map<std::string, iq::tile_ptr> &tiles)
	{
		this->load(path, tiles);
	}

	void tilemap::draw(const iq::uint i, const iq::BITMAP_ptr scrbuf, const iq::entity_ptr player) const
	{
		int map_x, map_y;
		iq::BITMAP_ptr bitmap;

		if(i >= this->layers.size())
		{
			throw std::runtime_error(
				"Invalid tilemap layer index '"
				+ boost::lexical_cast<std::string>(i)
				+ "'. Valid indexes are 0-"
				+ boost::lexical_cast<std::string>(this->layers.size()-1)
				+ ".");
		}

		map_y = iq::tilemap::screen_y(scrbuf, player);

		for(int y=0, ylen=this->layers[i].size(); y<ylen; y++)
		{
			map_x = iq::tilemap::screen_x(scrbuf, player);

			for(int x=0, xlen=this->layers[i][y].size(); x<xlen; x++)
			{
				bitmap = this->layers[i][y][x]->bitmap;
				masked_blit(bitmap.get(), scrbuf.get(), 0, 0, map_x + (x*this->tilesize), map_y + (y*this->tilesize), bitmap->w, bitmap->h);
			}
		}
	}

	const std::vector<tilemap::tilelayer> tilemap::get_layers(void) const
	{
		return this->layers;
	}

	const iq::uint tilemap::get_tilesize(void) const
	{
		return this->tilesize;
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

this->m_w = w;
this->m_h = h;

		this->load_layout(layout, path, tileset, num_layers, w, h, tiles);
	}

	void tilemap::load_collision(const TiXmlElement * const collision, const iq::uint w, const iq::uint h)
	{
		const TiXmlElement *element = NULL;

		if((element = collision->FirstChildElement("RowInfo")) == NULL)
			throw std::runtime_error("CollisionLayer XML is missing one or more RowInfo elements.");

		do
		{
			this->passable.push_back(this->load_collision_rowinfo(element, w));
		}while((element = element->NextSiblingElement("RowInfo")));
	}

	const std::vector<bool> tilemap::load_collision_rowinfo(const TiXmlElement * const rowinfo, const iq::uint w)
	{
		bool b;
		const TiXmlNode *node = NULL;
		std::vector<bool> passable_row;
		std::stringstream ss;

		if((node = rowinfo->FirstChild()) == NULL || node->Type() != TiXmlNode::TEXT)
			throw std::runtime_error("CollisionLayer RowInfo XML is invalid.");

		ss.clear();
		ss.str(node->ToText()->ValueStr());

		while(ss >> b)
		{
			passable_row.push_back(b);
			ss.ignore(1);
		}

		if(passable_row.size() != w)
		{
			throw std::runtime_error(
				"RowInfo XML tile count '"
				+ boost::lexical_cast<std::string>(passable_row.size())
				+ "' does not match defined Width '"
				+ boost::lexical_cast<std::string>(w)
				+ "'.");
		}

		return passable_row;
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
	}

	const iq::tilemap::tilelayer tilemap::load_layer(const TiXmlElement * const layer, const std::string &path, const iq::BITMAP_ptr tileset, const iq::uint w, const iq::uint h, std::map<std::string, iq::tile_ptr> &tiles)
	{
		const TiXmlElement *element = NULL;
		iq::tilemap::tilelayer l;
		std::string tag_name;

		if((element = layer->FirstChildElement()) == NULL)
			throw std::runtime_error("Layer XML missing one or more RowInfo elements.");

		do
		{
			tag_name = element->ValueStr();

			if(tag_name == "RowInfo")
				l.push_back(this->load_layer_rowinfo(element, path, tileset, w, tiles));
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

	const std::vector<iq::tile_ptr> tilemap::load_layer_rowinfo(const TiXmlElement * const rowinfo, const std::string &path, const BITMAP_ptr tileset, const iq::uint w, std::map<std::string, iq::tile_ptr> &tiles)
	{
		long i;
		const TiXmlNode *node = NULL;
		std::stringstream ss;
		std::vector<iq::tile_ptr> tilerow;

		if((node = rowinfo->FirstChild()) == NULL || node->Type() != TiXmlNode::TEXT)
			throw std::runtime_error("Layer RowInfo XML is invalid.");

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

	void tilemap::load_layout(const TiXmlElement * const layout, const std::string &path, const iq::BITMAP_ptr tileset, const iq::uint num_layers, const iq::uint w, const iq::uint h, std::map<std::string, iq::tile_ptr> &tiles)
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
				this->load_collision(element, w, h);
		}while((element = element->NextSiblingElement()));

		if(this->layers.size() != num_layers)
			throw std::runtime_error("Tilemap XML Layers count does not match the number of Layer elements.");
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

	const bool tilemap::is_passable(const iq::uint x, const iq::uint y) const
	{
		if(y >= this->passable.size() || x >= this->passable[y].size())
		{
			// throw std::runtime_error("Invalid passable tile coordinate '("
				// + boost::lexical_cast<std::string>(x)
				// + ","
				// + boost::lexical_cast<std::string>(y)
				// + ")'.");
			return false;
		}
		
			

		return this->passable[y][x];
	}

	const int tilemap::screen_x(const iq::BITMAP_ptr scrbuf, const iq::entity_ptr player)
	{
		return -(player->x()-(scrbuf->w/2.0));
	}

	const int tilemap::screen_y(const iq::BITMAP_ptr scrbuf, const iq::entity_ptr player)
	{
		return -(player->y()-(scrbuf->h/2.0));
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

	const iq::uint tilemap::h(void) const
	{
		return this->m_h;
	}

	const iq::uint tilemap::w(void) const
	{
		return this->m_w;
	}
}

