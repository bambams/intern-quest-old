
#include <animation.hpp>

namespace iq
{
	animation::animation(const boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i):
		ms_per_frame(0),
		last_ms(0)
	{
		this->load(sheet, i);
	}

	animation::animation(const unsigned int ms_per_frame, const boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i):
		last_ms(0)
	{
		this->set_ms_per_frame(ms_per_frame);
		this->load(ms_per_frame, sheet, i);
	}

	animation::animation(const boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i, const unsigned int anim_w, const unsigned int anim_h):
		ms_per_frame(0),
		last_ms(0)
	{
		this->load(sheet, i, anim_w, anim_h);
	}

	animation::animation(const unsigned int ms_per_frame, const boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i, const unsigned int anim_w, const unsigned int anim_h):
		last_ms(0)
	{
		this->set_ms_per_frame(ms_per_frame);
		this->load(ms_per_frame, sheet, i, anim_w, anim_h);
	}

	void animation::check_frame_index(const unsigned int i) const
	{
		if(i >= this->frames.size())
			throw std::range_error("Animation frame index out of range.");
	}

	void animation::check_ms(const unsigned int ms) const
	{
		if(this->last_ms > ms)
			throw std::invalid_argument("The current time exceeds the last animation frame time.");
	}

	void animation::check_ms_per_frame(void) const
	{
		if(this->ms_per_frame == 0)
			throw std::logic_error("iq::animation::ms_per_frame is uninitialized.");
	}

	const boost::shared_ptr<BITMAP> animation::begin(const unsigned int ms)
	{
		this->check_ms_per_frame();
		this->set_last_ms(ms);

		return(this->frames[0]);
	}

	const boost::shared_ptr<BITMAP> animation::begin(const unsigned int ms_per_frame, const unsigned int ms)
	{
		this->set_ms_per_frame(ms_per_frame);
		this->set_last_ms(ms);

		return(this->frames[0]);
	}

	const boost::shared_ptr<BITMAP> animation::frame(const unsigned int i) const
	{
		this->check_frame_index(i);

		return(this->frames[i]);
	}

	const unsigned int animation::height(void) const
	{
		return(this->frames[0]->h);
	}

	void animation::load(const boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i)
	{
		unsigned int anim_h = sheet->bitmap->h / sheet->h;
		unsigned int anim_w = sheet->bitmap->w / sheet->w;

		this->load(sheet, i, anim_w, anim_h);
	}

	void animation::load(const unsigned int ms_per_frame, const boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i)
	{
		this->set_ms_per_frame(ms_per_frame);
		this->load(sheet, i);
	}

	void animation::load(const boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i, const unsigned int anim_w, const unsigned int anim_h)
	{
		boost::shared_ptr<BITMAP> frame;
		int h, w;
		int y;

		if(i >= sheet->h)
			throw std::range_error("Spritesheet animation index out of range.");

		h = sheet->bitmap->h / sheet->h;
		w = sheet->bitmap->w / sheet->w;

		if(w == 0 || h == 0)
			throw std::invalid_argument("Spritesheet has invalid dimensions for an animation.");

		this->frames.resize(sheet->w);
		y = h * i;

		for(unsigned int j=0, x=0; j<sheet->w; j++, x += w)
		{
			frame.reset(create_bitmap(anim_w, anim_h), destroy_bitmap);

			if(frame.get() == NULL)
				throw std::runtime_error("Memory allocation failed.");

			stretch_blit(sheet->bitmap.get(), frame.get(), x, y, w, h, 0, 0, anim_w, anim_h);

			this->frames[j] = frame;
		}
	}

	void animation::load(const unsigned int ms_per_frame, const boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i, const unsigned int anim_w, const unsigned int anim_h)
	{
		this->set_ms_per_frame(ms_per_frame);
		this->load(sheet, i, anim_w, anim_h);
	}

	const boost::shared_ptr<BITMAP> animation::next(const unsigned int ms)
	{
		unsigned int i;
		unsigned int past;

		this->check_ms_per_frame();
		this->check_ms(ms);

		past = (ms - this->last_ms) / this->ms_per_frame;
		i = (this->last_frame + past) % this->frames.size();

		this->set_last_ms(ms);

		return(this->frames[i]);
	}

	const boost::shared_ptr<BITMAP> animation::operator[](const unsigned int i) const
	{
		return(this->frame(i));
	}

	void animation::set_last_ms(const unsigned int ms)
	{
		this->check_ms_per_frame();
		this->check_ms(ms);
		this->last_ms = ms;
	}

	void animation::set_ms_per_frame(const unsigned int ms_per_frame)
	{
		if(ms_per_frame == 0)
			throw std::range_error("0 is not a valid value for iq::animation::ms_per_frame.");

		this->ms_per_frame = ms_per_frame;
	}

	const unsigned int animation::width(void) const
	{
		return(this->frames[0]->w);
	}
}

