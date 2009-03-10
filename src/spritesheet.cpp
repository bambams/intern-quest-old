
#include <spritesheet.hpp>

namespace iq
{
	spritesheet::spritesheet(const std::string &path, const unsigned int w, const unsigned int h):
		h(h),
		w(w)
	{
		this->load(path, w, h);
	}

	spritesheet::spritesheet(const boost::shared_ptr<BITMAP> bitmap, const unsigned int w, const unsigned int h):
		bitmap(bitmap),
		h(h),
		w(w)
	{
	}

	void spritesheet::load(const std::string &path, const unsigned int w, const unsigned int h)
	{
		this->bitmap.reset(load_bitmap(path.c_str(), NULL), destroy_bitmap);

		if(this->bitmap.get() == NULL)
			throw std::runtime_error("Failed to load spritesheet.");
	}
}

