
#ifndef IQ_ANIMATION_HPP
	#define IQ_ANIMATION_HPP

namespace iq
{
	class animation;
}

	//#include <frame.hpp>
	#include <spritesheet.hpp>
	#include <stdexcept>
	#include <vector>

namespace iq
{
	class animation
	{
	private:
	protected:
		//std::vector< boost::shared_ptr<iq::frame> > frames;
		std::vector< boost::shared_ptr<BITMAP> > frames;
	public:
		animation(boost::shared_ptr<iq::spritesheet>, const unsigned int);
		animation(boost::shared_ptr<iq::spritesheet>, const unsigned int, const unsigned int, const unsigned int);

		boost::shared_ptr<BITMAP> frame(const unsigned int) const;
		void load(boost::shared_ptr<iq::spritesheet>, const unsigned int);
		void load(boost::shared_ptr<iq::spritesheet>, const unsigned int, const unsigned int, const unsigned int);

		boost::shared_ptr<BITMAP> operator[](const unsigned int) const;
	};
}

#endif

