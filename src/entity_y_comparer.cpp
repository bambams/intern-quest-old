#include <entity_y_comparer.hpp>

namespace iq
{
	bool entity_y_comparer::operator()(iq::entity_ptr first, iq::entity_ptr second)
	{
		return first->y() < second->y();
	}
}

