
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
		std::vector<tilelayer> layers;
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
		tilemap(const std::string &, std::map<std::string, iq::tile_ptr> &);

		void draw(const iq::BITMAP_ptr, const iq::entity_ptr) const;
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

