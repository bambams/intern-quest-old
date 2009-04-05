#include <entity_y_comparer.hpp>

namespace iq
{
	bool entity_y_comparer::operator()(const iq::entity_ptr first, const iq::entity_ptr second) const
	{
		return first->y() < second->y();
	}
}

