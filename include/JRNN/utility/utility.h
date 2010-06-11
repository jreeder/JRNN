/* 
 * File:   utility.h
 * Author: jreeder
 * 
 * Created on June 10, 2010, 9:00 PM
 */
#ifndef _UTILITY_H
#define _UTILITY_H

namespace JRNN {

	inline vecDouble SquareVec(vecDouble vector){
		vecDouble::iterator it = vector.begin();
		for(;it < vector.end(); it++){
			(*it) = pow((*it),2);
		}
		return vector;
	}
	inline vecDouble ApplyThreshold(vecDouble vector){
		vecDouble::iterator it = vector.begin();
		for(;it != vector.end(); it++){
			if ((*it) < 0.5){
				(*it) = 0;
			}
			else{
				(*it) = 1;
			}
		}
		return vector;
	}

	inline int Sign(double num){
		if (num > 0.0){
			return 1;
		}
		else if (num < 0.0){
			return -1;
		}
		else{
			return 0;
		}
	}
}

#endif