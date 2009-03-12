
#ifndef IQ_ANIMATION_HPP
	#define IQ_ANIMATION_HPP

namespace iq
{
	class animation;
}

	#include <boost/shared_ptr.hpp>
	#include <spritesheet.hpp>
	#include <stdexcept>
	#include <tinyxml.h>
	#include <types.hpp>
	#include <vector>

namespace iq
{
	typedef boost::shared_ptr<animation> animation_ptr;

	class animation
	{
	private:
	protected:
		iq::uint m_ms_per_frame;
		iq::uint m_last_ms;
		iq::uint m_last_frame;
		std::vector<iq::BITMAP_ptr> m_frames;

		void check_frame_index(const iq::uint) const;
		void check_ms(const iq::uint) const;
		void check_ms_per_frame(void) const;
		void set_last_frame(const iq::uint);
		void set_last_ms(const iq::uint);
		void set_ms_per_frame(const iq::uint);
	public:
		animation(const spritesheet_ptr, const iq::uint);
		animation(const spritesheet_ptr, const iq::uint, const iq::uint);
		animation(const spritesheet_ptr, const iq::uint, const iq::uint, const iq::uint);
		animation(const spritesheet_ptr, const iq::uint, const iq::uint, const iq::uint, const iq::uint);
		animation(const TiXmlElement *, iq::spritesheet_ptr, iq::uint, iq::uint_ptr, iq::uint_ptr, iq::uint_ptr);

		const iq::BITMAP_ptr begin(const iq::uint);
		const iq::BITMAP_ptr begin(const iq::uint, const iq::uint);
		const iq::BITMAP_ptr frame(const iq::uint) const;
		const iq::uint h(void) const;
		void load(const spritesheet_ptr, const iq::uint);
		void load(const spritesheet_ptr, const iq::uint, const iq::uint);
		void load(const spritesheet_ptr, const iq::uint, const iq::uint, const iq::uint);
		void load(const spritesheet_ptr, const iq::uint, const iq::uint, const iq::uint, const iq::uint);
		void load(const TiXmlElement*, spritesheet_ptr, iq::uint, uint_ptr, uint_ptr, uint_ptr);
		const iq::BITMAP_ptr next(const iq::uint);
		const iq::uint w(void) const;

		const iq::BITMAP_ptr operator[](const iq::uint) const;
	};
}

#endif

