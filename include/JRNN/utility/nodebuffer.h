/* 
 * File:   nodebuffer.h
 * Author: jreeder
 * 
 * Created on June 9, 2010, 9:00 PM
 */

#ifndef _NODEBUFFER_H
#define _NODEBUFFER_H

#include "JRNN.h"

namespace JRNN {

	class NodeBuffer
	{
	public:
		NodeBuffer(void);
		~NodeBuffer(void);
		std::vector<double>& operator[] (std::string key);

		void Clear();

	private:
		boost::unordered_map<std::string, std::vector<double> > buffer;
	};

}
#endif

