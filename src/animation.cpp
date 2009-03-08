
#include <animation.hpp>

namespace iq
{
	animation::animation(boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i)
	{
		this->load(sheet, i);
	}

	animation::animation(boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i, const unsigned int anim_w, const unsigned int anim_h)
	{
		this->load(sheet, i, anim_w, anim_h);
	}

	boost::shared_ptr<BITMAP> animation::frame(const unsigned int i) const
	{
		if(i >= this->frames.size())
			throw new std::range_error("Animation frame index out of range.");

		return(this->frames[i]);
	}

	void animation::load(boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i)
	{
		unsigned int h = sheet->bitmap->h / sheet->h;
		unsigned int w = sheet->bitmap->w / sheet->w;

		this->load(sheet, i, w, h);
	}

	void animation::load(boost::shared_ptr<iq::spritesheet> sheet, const unsigned int i, const unsigned int anim_w, const unsigned int anim_h)
	{
		boost::shared_ptr<BITMAP> frame;
		int h, w;
		int y;

		if(i >= sheet->h)
			throw std::range_error("Spritesheet animation index out of range.");

		this->frames.resize(sheet->w);

		h = sheet->bitmap->h / sheet->h;
		w = sheet->bitmap->w / sheet->w;
		y = h * i;

		for(unsigned int j=0, x=0; j<sheet->w; j++, x += w)
		{
			frame.reset(create_bitmap(anim_w, anim_h), destroy_bitmap);

			if(frame.get() == NULL)
				throw new std::runtime_error("Memory allocation failed.");

			stretch_blit(sheet->bitmap.get(), frame.get(), x, y, w, h, 0, 0, anim_w, anim_h);

			this->frames[j] = frame;
		}
	}

	boost::shared_ptr<BITMAP> animation::operator[](const unsigned int i) const
	{
		return(this->frame(i));
	}
}

