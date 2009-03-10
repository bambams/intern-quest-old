
#ifndef IQ_ANIMATION_HPP
	#define IQ_ANIMATION_HPP

namespace iq
{
	class animation;
}

	#include <spritesheet.hpp>
	#include <stdexcept>
	#include <vector>

namespace iq
{
	class animation
	{
	private:
	protected:
		unsigned int m_ms_per_frame;
		unsigned int m_last_ms;
		unsigned int m_last_frame;
		std::vector< boost::shared_ptr<BITMAP> > m_frames;

		void check_frame_index(const unsigned int) const;
		void check_ms(const unsigned int) const;
		void check_ms_per_frame(void) const;
		void set_last_frame(const unsigned int);
		void set_last_ms(const unsigned int);
		void set_ms_per_frame(const unsigned int);
	public:
		animation(const boost::shared_ptr<iq::spritesheet>, const unsigned int);
		animation(const unsigned int, const boost::shared_ptr<iq::spritesheet>, const unsigned int);
		animation(const boost::shared_ptr<iq::spritesheet>, const unsigned int, const unsigned int, const unsigned int);
		animation(const unsigned int, const boost::shared_ptr<iq::spritesheet>, const unsigned int, const unsigned int, const unsigned int);

		const boost::shared_ptr<BITMAP> begin(const unsigned int);
		const boost::shared_ptr<BITMAP> begin(const unsigned int, const unsigned int);
		const boost::shared_ptr<BITMAP> frame(const unsigned int) const;
		const unsigned int h(void) const;
		void load(const boost::shared_ptr<iq::spritesheet>, const unsigned int);
		void load(const unsigned int, const boost::shared_ptr<iq::spritesheet>, const unsigned int);
		void load(const boost::shared_ptr<iq::spritesheet>, const unsigned int, const unsigned int, const unsigned int);
		void load(const unsigned int, const boost::shared_ptr<iq::spritesheet>, const unsigned int, const unsigned int, const unsigned int);
		const boost::shared_ptr<BITMAP> next(const unsigned int);
		const unsigned int w(void) const;

		const boost::shared_ptr<BITMAP> operator[](const unsigned int) const;
	};
}

#endif

