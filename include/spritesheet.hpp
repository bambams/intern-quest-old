
#ifndef IQ_SPRITESHEET_HPP
	#define IQ_SPRITESHEET_HPP

namespace iq
{
	class spritesheet;
}

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <stdexcept>
	#include <string>
	#include <types.hpp>

namespace iq
{
	typedef boost::shared_ptr<spritesheet> spritesheet_ptr;

	class spritesheet
	{
	private:
	protected:
	public:
		spritesheet(const std::string &, const iq::uint, const iq::uint);
		spritesheet(const iq::BITMAP_ptr, const iq::uint, const iq::uint);

		iq::BITMAP_ptr bitmap;
		iq::uint h;
		iq::uint w;

		void load(const std::string &, const iq::uint, const iq::uint);
	};
}

#endif

