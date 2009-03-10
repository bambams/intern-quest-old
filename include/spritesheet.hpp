
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

namespace iq
{
	class spritesheet
	{
	private:
	protected:
	public:
		spritesheet(const std::string &, const unsigned int, const unsigned int);
		spritesheet(const boost::shared_ptr<BITMAP>, const unsigned int, const unsigned int);

		boost::shared_ptr<BITMAP> bitmap;
		unsigned int h;
		unsigned int w;

		void load(const std::string &, const unsigned int, const unsigned int);
	};
}

#endif

