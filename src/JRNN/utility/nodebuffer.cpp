#include "JRNN.h"
#include "utility/nodebuffer.h"

namespace JRNN {

	NodeBuffer::NodeBuffer(void)
	{
	}

	NodeBuffer::~NodeBuffer(void)
	{
	}

	std::vector<double>& NodeBuffer::operator[]( std::string key )
	{
		return buffer[key];
	}

	void NodeBuffer::Clear()
	{
		buffer.clear();
	}

}