
#include <animation.hpp>

namespace iq
{
	animation::animation(const iq::spritesheet_ptr sheet, const iq::uint i):
		m_ms_per_frame(0)
	{
		this->reset();
		this->load(sheet, i);
	}

	animation::animation(const iq::spritesheet_ptr sheet, const iq::uint i, const iq::uint ms_per_frame)
	{
		this->reset();
		this->set_ms_per_frame(ms_per_frame);
		this->load(sheet, i, ms_per_frame);
	}

	animation::animation(const iq::spritesheet_ptr sheet, const iq::uint i, const iq::uint w, const iq::uint h):
		m_ms_per_frame(0)
	{
		this->reset();
		this->load(sheet, i, w, h);
	}

	animation::animation(const iq::spritesheet_ptr sheet, const iq::uint i, const iq::uint w, const iq::uint h, const iq::uint ms_per_frame)
	{
		this->reset();
		this->set_ms_per_frame(ms_per_frame);
		this->load(sheet, i, w, h, ms_per_frame);
	}

	animation::animation(const TiXmlElement * const animation, const iq::spritesheet_ptr sheet, const iq::uint i, const iq::uint_ptr w, const iq::uint_ptr h, const iq::uint_ptr ms_per_frame)
	{
		this->reset();
		this->load(animation, sheet, i, w, h, ms_per_frame);
	}

	void animation::check_frame_index(const iq::uint i) const
	{
		if(i >= this->m_frames.size())
			throw std::range_error("Animation frame index out of range.");
	}

	void animation::check_ms(const iq::uint ms) const
	{
		if(this->m_last_ms > ms)
			throw std::invalid_argument("The current time exceeds the last animation frame time.");
	}

	void animation::check_ms_per_frame(void) const
	{
		if(this->m_ms_per_frame == 0)
			throw std::logic_error("iq::animation::ms_per_frame is uninitialized.");
	}

	const iq::BITMAP_ptr animation::begin(const iq::uint ms)
	{
		this->check_ms_per_frame();
		this->set_last_ms(ms);
		this->set_last_frame(0);

		this->m_playing = true;

		if(this->m_frames.size() > 1)
			return this->m_frames[this->m_last_frame = 1];
		else
			return this->m_frames[this->m_last_frame = 0];
	}

	const iq::BITMAP_ptr animation::begin(const iq::uint ms_per_frame, const iq::uint ms)
	{
		this->set_ms_per_frame(ms_per_frame);
		this->set_last_ms(ms);

		this->m_playing = true;

		if(this->m_frames.size() > 1)
			return this->m_frames[this->m_last_frame = 1];
		else
			return this->m_frames[this->m_last_frame = 0];
	}

	const iq::BITMAP_ptr animation::frame(const iq::uint i) const
	{
		this->check_frame_index(i);

		return this->m_frames[i];
	}

	const iq::uint animation::h(void) const
	{
		return this->m_frames[0]->h;
	}

	void animation::load(const iq::spritesheet_ptr sheet, const iq::uint i)
	{
		iq::uint anim_h = sheet->bitmap->h / sheet->h;
		iq::uint anim_w = sheet->bitmap->w / sheet->w;

		this->load(sheet, i, anim_w, anim_h);
	}

	void animation::load(const iq::spritesheet_ptr sheet, const iq::uint i, const iq::uint ms_per_frame)
	{
		this->set_ms_per_frame(ms_per_frame);
		this->load(sheet, i);
	}

	void animation::load(const iq::spritesheet_ptr sheet, const iq::uint i, const iq::uint w, const iq::uint h)
	{
		iq::BITMAP_ptr frame;
		int sprite_h, sprite_w;
		int y;

		if(i >= sheet->h)
			throw std::range_error("Spritesheet animation index out of range.");

		sprite_h = sheet->bitmap->h / sheet->h;
		sprite_w = sheet->bitmap->w / sheet->w;

		if(sprite_w == 0 || sprite_h == 0)
			throw std::invalid_argument("Spritesheet has invalid dimensions for an animation.");

		this->m_frames.resize(sheet->w);
		y = sprite_h * i;

		for(iq::uint j=0, x=0; j<sheet->w; j++, x += sprite_w)
		{
			frame.reset(create_bitmap(w, h), destroy_bitmap);

			if(frame.get() == NULL)
				throw std::runtime_error("Memory allocation failed.");

			stretch_blit(sheet->bitmap.get(), frame.get(), x, y, sprite_w, sprite_h, 0, 0, w, h);

//for(iq::uint k=0; k<3; k++)
//rect(frame.get(), 0+k, 0+k, frame->w-k-1, frame->h-k-1, makecol(155, 255, 155));

			this->m_frames[j] = frame;
		}
	}

	void animation::load(const iq::spritesheet_ptr sheet, const iq::uint i, const iq::uint w, const iq::uint h, const iq::uint ms_per_frame)
	{
		this->set_ms_per_frame(ms_per_frame);
		this->load(sheet, i, w, h);
	}

	void animation::load(const TiXmlElement * const animation, const iq::spritesheet_ptr sheet, const iq::uint i, const iq::uint_ptr w, const iq::uint_ptr h, const iq::uint_ptr ms_per_frame)
	{
		if(animation->Attribute("ms_per_frame", (int *)&this->m_ms_per_frame) == NULL)
		{
			if(ms_per_frame.get() == NULL)
				throw std::runtime_error("Entity XML spritesheet element and animation element missing ms_per_frame attribute.");
			else
				this->m_ms_per_frame = *ms_per_frame;
		}
		else
		{
			this->set_ms_per_frame(this->m_ms_per_frame);
		}

		if((h.get() == NULL) != (w.get() == NULL))
			throw std::runtime_error("Entity XML spritesheet attributes anim_width and anim_height must either both exist or neither can exist.");
		else if(h.get() == NULL)
			this->load(sheet, i);
		else
			this->load(sheet, i, *w, *h, *ms_per_frame);
	}

	const iq::BITMAP_ptr animation::next(const iq::uint ms)
	{
		iq::uint i;
		iq::uint past;

		if(!this->m_playing)
			return this->m_frames[this->m_last_frame];

		this->check_ms_per_frame();
		this->check_ms(ms);

		past = (ms - this->m_last_ms) / this->m_ms_per_frame;
		i = (this->m_last_frame + past) % this->m_frames.size();

//printf("ms=%d    last_ms=%d    past=%d    i=%d\n", ms, this->m_last_ms, past, i);

		if(i != this->m_last_frame)
		{
			this->set_last_ms(ms);
			this->set_last_frame(i);
		}

		return this->m_frames[i];
	}

	const iq::BITMAP_ptr animation::operator[](const iq::uint i) const
	{
		return this->frame(i);
	}

	void animation::pause(void)
	{
		this->m_playing = false;
	}

	const bool animation::playing(void) const
	{
		return this->m_playing;
	}

	void animation::reset(void)
	{
		this->pause();
		this->m_last_frame = 0;
		this->m_last_ms = 0;
	}

	void animation::set_last_frame(const iq::uint i)
	{
		this->check_frame_index(i);
		this->m_last_frame = i;
	}

	void animation::set_last_ms(const iq::uint ms)
	{
		this->check_ms_per_frame();
		this->check_ms(ms);
		this->m_last_ms = ms;
	}

	void animation::set_ms_per_frame(const iq::uint ms_per_frame)
	{
		if(ms_per_frame == 0)
			throw std::range_error("0 is not a valid value for iq::animation::ms_per_frame.");

		this->m_ms_per_frame = ms_per_frame;
	}

	const iq::uint animation::w(void) const
	{
		return this->m_frames[0]->w;
	}
}

