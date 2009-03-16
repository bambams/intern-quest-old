
#ifndef IQ_TYPES_HPP
	#define IQ_TYPES_HPP

	#include <allegro.h>
	#include <boost/shared_ptr.hpp>
	#include <map>
	#include <semaphore.h>
	#include <tinyxml.h>
	#include <vector>

namespace iq
{
	typedef unsigned int uint;

	typedef std::map<std::string, std::string> string_map;
	typedef std::vector<std::string> string_vector;

	typedef boost::shared_ptr<BITMAP> BITMAP_ptr;
	typedef boost::shared_ptr<TiXmlDocument> TiXmlDocument_ptr;
	typedef boost::shared_ptr<sem_t> sem_t_ptr;
	typedef boost::shared_ptr<string_map> string_map_ptr;
	typedef boost::shared_ptr<string_vector> string_vector_ptr;
	typedef boost::shared_ptr<uint> uint_ptr;
}

#endif

