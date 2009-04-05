#ifndef ENTITY_Y_COMPARER
	#define ENTITY_Y_COMPARER

namespace iq
{
	struct entity_y_comparer;
}

	#include <entity.hpp>

namespace iq
{
	struct entity_y_comparer
	{
		bool operator()(const iq::entity_ptr, const iq::entity_ptr) const;
	};
}

#endif

