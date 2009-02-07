
#ifndef IQ_IQ_APP
	#define IQ_IQ_APP

	#include <map>
	#include <string>

namespace iq
{
	/*
	 * \brief An application class to hold application state data.
	 * \details Intended to be passed around the application to avoid
	 *          global state. Any sort of data that needs to be available
	 *          to the application that comes from far away can be attached
	 *          to this class... >_> A first-attempt at avoiding global
	 *          state
	 */
	class app
	{
	private:
		
	protected:
		std::map<std::string, std::string> argv;
	public:
		bool os_cursor;

		app(int, char *[]);

		void parse_args(int argc, char *[]);
	};
}

#endif

