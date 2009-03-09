
#ifndef IQ_ENTITY_HPP
	#define IQ_ENTITY_HPP

namespace iq
{
	class entity;
}

	#include <animation.hpp>
	#include <boost/algorithm/string/trim.hpp>
	#include <boost/shared_ptr.hpp>
	#include <map>
	#include <spritesheet.hpp>
	#include <string>
	#include <tinyxml.h>
	#include <vector>

namespace iq
{
	class entity
	{
		typedef std::map<std::string, boost::shared_ptr<iq::animation> > animation_map;
	private:
	protected:
		animation_map::const_iterator current_animation_iterator;
		boost::shared_ptr<animation_map> animations;

		void load_animation(const unsigned int &, const TiXmlElement * const, const boost::shared_ptr<iq::spritesheet>, const boost::shared_ptr<unsigned int>);
		void load_spritesheet(const TiXmlElement * const);
	public:
		unsigned int h, w;
		int x, y;

		entity(const std::string &);
		entity(const std::string &, const unsigned int, const unsigned int);

		const boost::shared_ptr< std::vector<std::string> > animation_keys(void) const;
		const animation_map::const_iterator begin_animation(const std::string &, const unsigned int);
		const boost::shared_ptr<BITMAP> current_frame(const unsigned int) const;
		const animation_map::const_iterator current_animation(void) const;
		void load(const std::string &);
	};
}

#endif

