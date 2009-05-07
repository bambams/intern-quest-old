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


#ifndef IQ_TILEMAP_HPP
	#define IQ_TILEMAP_HPP

	#include <boost/lexical_cast.hpp>
	#include <boost/shared_ptr.hpp>
	#include <stdexcept>
	#include <string>
	#include <vector>

namespace iq
{
	class tilemap;
	typedef boost::shared_ptr<tilemap> tilemap_ptr;
}
	#include <entity.hpp>
	#include <tile.hpp>

namespace iq
{
	class tilemap
	{
	private:
	protected:
		typedef std::vector< std::vector<tile_ptr > > tilelayer;

		std::vector< std::vector<bool> > passable;
		iq::uint tilesize;
		iq::uint m_w, m_h;

		void load_collision(const TiXmlElement * const, const iq::uint, const iq::uint);
		const std::vector<bool> load_collision_rowinfo(const TiXmlElement * const, const iq::uint);
		void load_dimensions(const TiXmlElement * const, iq::uint &, iq::uint &, iq::uint &);
		const tilelayer load_layer(const TiXmlElement * const, const std::string &, const iq::BITMAP_ptr, const iq::uint, const iq::uint, std::map<std::string, iq::tile_ptr> &);
		const std::vector<iq::tile_ptr> load_layer_rowinfo(const TiXmlElement * const, const std::string &, const iq::BITMAP_ptr, const iq::uint, std::map<std::string, iq::tile_ptr> &);
		void load_layout(const TiXmlElement * const, const std::string &, const iq::BITMAP_ptr, const iq::uint, const iq::uint, const iq::uint, std::map<std::string, iq::tile_ptr> &);
		const iq::tile_ptr load_tile(std::map<std::string, iq::tile_ptr> &, const BITMAP_ptr, const std::string &, const iq::uint);
		const iq::BITMAP_ptr load_tileset(const TiXmlElement * const);
		const iq::tile_ptr void_tile(std::map<std::string, iq::tile_ptr> &) const;
	public:
		std::vector<tilelayer> layers;

		tilemap(const std::string &, std::map<std::string, iq::tile_ptr> &);

		void draw(const iq::uint, const iq::BITMAP_ptr, const iq::entity_ptr) const;
		const iq::uint h(void) const;
		const std::vector<tilelayer> get_layers(void) const;
		const iq::uint get_tilesize(void) const;
		const bool is_passable(const iq::uint, const iq::uint) const;
		void load(const std::string &, std::map<std::string, iq::tile_ptr> &);
		void load(const TiXmlElement * const, const std::string &, std::map<std::string, iq::tile_ptr> &);
		const iq::uint w(void) const;
		//bool collision(int x, int y);	// x, y are tile coordinates, not pixel coordinates!

		static const int screen_x(const iq::BITMAP_ptr, const iq::entity_ptr);
		static const int screen_y(const iq::BITMAP_ptr, const iq::entity_ptr);
	};
}

#endif

