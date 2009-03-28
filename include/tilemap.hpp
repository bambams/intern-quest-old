
#ifndef IQ_TILEMAP_HPP
	#define IQ_TILEMAP_HPP

namespace iq
{
	class tilemap;
}
	#include <boost/lexical_cast.hpp>
	#include <boost/shared_ptr.hpp>
	#include <entity.hpp>
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
		typedef std::vector< std::vector<tile_ptr > > tilelayer;

		std::vector<tilelayer> layers;
		iq::uint tilesize;

		void load_dimensions(const TiXmlElement * const, iq::uint &, iq::uint &, iq::uint &);
		const std::vector< std::vector<iq::tile_ptr> > load_layer(const TiXmlElement * const, const std::string &, const iq::BITMAP_ptr, const iq::uint &, const iq::uint &, std::map<std::string, iq::tile_ptr> &);
		void load_layout(const TiXmlElement * const, const std::string &, const iq::BITMAP_ptr, const iq::uint &, const iq::uint &, const iq::uint &, std::map<std::string, iq::tile_ptr> &);
		const std::vector<iq::tile_ptr> load_rowinfo(const TiXmlElement * const, const std::string &, const iq::BITMAP_ptr, const iq::uint &, std::map<std::string, iq::tile_ptr> &);
		const iq::tile_ptr load_tile(std::map<std::string, iq::tile_ptr> &, const BITMAP_ptr, const std::string &, const iq::uint);
		const iq::BITMAP_ptr load_tileset(const TiXmlElement * const);
		const iq::tile_ptr void_tile(std::map<std::string, iq::tile_ptr> &) const;
	public:
		tilemap(const std::string &, std::map<std::string, iq::tile_ptr> &);

		void draw(const iq::BITMAP_ptr, const iq::entity_ptr) const;
		const unsigned int h(void) const;
		const std::vector<tilelayer> get_layers(void) const;
		const iq::uint get_tilesize(void) const;
		void load(const std::string &, std::map<std::string, iq::tile_ptr> &);
		void load(const TiXmlElement * const, const std::string &, std::map<std::string, iq::tile_ptr> &);
//		void loadHard();
		const unsigned int w(void) const;
	};
}

#endif

