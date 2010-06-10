#include "JRNN.h"

namespace JRNN {

	NodeBuffer::NodeBuffer(void)
	{
	}

	NodeBuffer::~NodeBuffer(void)
	{
	}

	vector<double>& NodeBuffer::operator[]( std::string key )
	{
		return buffer[key];
	}

	void NodeBuffer::Clear()
	{
		buffer.clear();
	}

}