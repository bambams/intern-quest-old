
#include <spritesheet.hpp>

namespace iq
{
	spritesheet::spritesheet(const std::string &path, const iq::uint w, const iq::uint h):
		h(h),
		w(w)
	{
		this->load(path, w, h);
	}

	spritesheet::spritesheet(const iq::BITMAP_ptr bitmap, const iq::uint w, const iq::uint h):
		bitmap(bitmap),
		h(h),
		w(w)
	{
	}

	void spritesheet::load(const std::string &path, const iq::uint w, const iq::uint h)
	{
		this->h = h;
		this->w = w;

		this->bitmap.reset(load_bitmap(path.c_str(), NULL), destroy_bitmap);

		if(this->bitmap.get() == NULL)
			throw std::runtime_error("Failed to load spritesheet.");
	}
}

