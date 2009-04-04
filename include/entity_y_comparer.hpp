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
		bool operator()(iq::entity_ptr, iq::entity_ptr);
	};
}

#endif

