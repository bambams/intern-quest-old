
#include <animation.hpp>

namespace iq
{
	animation::animation(boost::shared_ptr<iq::spritesheet> sheet, const int i)
	{
		this->load(sheet, i);
	}

	void animation::load(boost::shared_ptr<iq::spritesheet> sheet, const int i)
	{
		//boost::shared_ptr<BITMAP> bitmap;
		boost::shared_ptr<BITMAP> frame;
		//boost::shared_ptr<iq::frame> frame;

		int h, w;
		int y;

		this->frames.resize(sheet->w);

		h = sheet->bitmap->h / sheet->h;
		w = sheet->bitmap->w / sheet->w;
		y = h * i;

		for(int j=0, x=0; j<sheet->w; j++, x += w * j)
		{
			//bitmap.reset(create_bitmap(w, h), destroy_bitmap);
			frame.reset(create_bitmap(w, h), destroy_bitmap);

			//if(bitmap.get() == NULL)
			if(frame.get() == NULL)
				throw new std::runtime_error("Memory allocation failed.");

			//blit(sheet->bitmap.get(), bitmap.get(), x, y, 0, 0, w, h);
			blit(sheet->bitmap.get(), frame.get(), x, y, 0, 0, w, h);

			//frame.reset(new iq::frame(bitmap));

			this->frames[j] = frame;
		}
	}

	boost::shared_ptr<BITMAP> animation::operator[](const unsigned int i) const
	{
		if(i >= this->frames.size())
			throw new std::range_error("Animation frame index out of range.");

		return(this->frames[i]);
	}
}

