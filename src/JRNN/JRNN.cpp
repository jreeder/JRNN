#include "JRNN.h"
//#undef NO_IMPORT_ARRAY
//#include <numpy/arrayobject.h>

namespace JRNN {
	rangeType StaticRange::gen = rangeType();
}

//namespace
//{
//	static struct pyublas_array_importer
//	{
//		static bool do_import_array()
//		{
//			import_array1(false);
//			return true;
//		}
//
//		pyublas_array_importer()
//		{
//			if (!do_import_array())
//				throw std::runtime_error("numpy failed to initialize");
//		}
//	} _array_importer;
//}