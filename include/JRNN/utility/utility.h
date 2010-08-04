/* 
 * File:   utility.h
 * Author: jreeder
 * 
 * Created on June 10, 2010, 9:00 PM
 */
#ifndef _UTILITY_H
#define _UTILITY_H

namespace JRNN {

	enum conType {
		IN,
		OUT
	};

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

	inline vecDouble VecMultiply(vecDouble v1, vecDouble v2){
		assert(v1.size() == v2.size());
		vecDouble result(v1.size());
		vecDouble::iterator it1 = v1.begin();
		vecDouble::iterator it2 = v2.begin();
		vecDouble::iterator res = result.begin();
		for (; it1 != v1.end(); it1++, it2++, res++){
			(*res) = (*it1) * (*it2);
		}
		return result;
	}

	inline void FillVec(vecDouble& vector, double value){
		vecDouble::iterator it = vector.begin();
		for (; it != vector.end(); it++){
			(*it) = value;
		}
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

	inline vecDouble Error(vecDouble first, vecDouble second){
		assert(first.size() == second.size());
		vecDouble result(first.size());
		vecDouble::iterator firstIT = first.begin();
		vecDouble::iterator secondIT = second.begin();
		vecDouble::iterator resultIT = result.begin();
		for (; firstIT != first.end(); firstIT++, secondIT++, resultIT++){
			if ((*firstIT) == UNKNOWN || (*secondIT) == UNKNOWN){
				(*resultIT) = 0;
			}
			else {
				(*resultIT) = (*firstIT) - (*secondIT);
			}
		}
		return result;
	}

	inline double Error(double first, double second){
		double result = 0;
		if (first != UNKNOWN && second != UNKNOWN){
			result = first - second;
		}
		return result;
	}

	class hashedVecDoubleMap {
	public:
		hashedVecDoubleMap(){
			vecSize = 0;
		}
		hashedVecDoubleMap(int size){
			vecSize = size;
		}

		hashedVecDoubleMap(const hashedVecDoubleMap& orig){
			storage = orig.storage;
			vecSize = orig.vecSize;
		}

		vecDouble& operator	[] (string name) {
			if(storage.find(name) != storage.end()){
				return storage[name];
			}
			else{
				storage[name] = vecDouble(vecSize);
				return storage[name];
			}
			
		}
		void clear() {
			storage.clear();
		}
	private:
		int vecSize;
		boost::unordered_map<string, vecDouble> storage;
	};

	template<class T>
	inline string StringFromVector(T& inVector){
		T::iterator it = inVector.begin();
		string outstring = "";
		bool firsttime = true;
		while (it != inVector.end()){
			if (firsttime == false){
				outstring += ",";
			}
			else {
				firsttime = false;
			}
			outstring += lexical_cast<string>((*it));
			it++;
		}
		return outstring;
	}
}

#endif