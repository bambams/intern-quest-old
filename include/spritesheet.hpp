
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
		spritesheet(std::string, const int, const int);
		spritesheet(boost::shared_ptr<BITMAP>, const int, const int);

		boost::shared_ptr<BITMAP> bitmap;
		int h;
		int w;

		void load(std::string, const int, const int);
	};
}

#endif

