#ifndef SELECT_VALUE_HPP
	#define SELECT_VALUE_HPP

namespace iq
{
	template<typename pair>
	struct select_value;
}

namespace iq
{
	template<typename pair>
	struct select_value
	{
		const typename pair::second_type &operator()(const pair &p) const
		{
			return p.second;
		}
	};
}

#endif

