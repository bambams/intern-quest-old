
#include <spritesheet.hpp>

namespace iq
{
	spritesheet::spritesheet(std::string path, const int w, const int h):
		h(h),
		w(w)
	{
		this->load(path, w, h);
	}

	spritesheet::spritesheet(boost::shared_ptr<BITMAP> bitmap, const int w, const int h):
		bitmap(bitmap),
		h(h),
		w(w)
	{
	}

	void spritesheet::load(std::string path, const int w, const int h)
	{
		this->bitmap.reset(load_bitmap(path.c_str(), NULL), destroy_bitmap);

		if(this->bitmap.get() == NULL)
			throw new std::runtime_error("Failed to load spritesheet.");
	}
}

