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

	inline vecDouble VecAddScalar(vecDouble& vector, double scalar){
		vecDouble result(vector.size());
		for (unsigned int i = 0; i < vector.size(); i++){
			result[i] = vector[i] + scalar;
		}
		return result;
	}

	inline vecDouble SquareVec(vecDouble& vector){
		//vecDouble::iterator it = vector.begin();
		vecDouble result(vector.size());
		for(unsigned int i = 0; i < vector.size(); i++){
			result[i] = pow((vector[i]),2);
		}
		return result;
	}
	inline vecDouble ApplyThreshold(vecDouble& vector){
		//vecDouble::iterator it = vector.begin();
		vecDouble result(vector.size());
		for(unsigned int i = 0; i < vector.size(); i++){
			if (vector[i] < 0.5){
				result[i] = 0;
			}
			else{
				result[i] = 1;
			}
		}
		return result;
	}

	inline vecDouble VecMultiply(vecDouble& v1, vecDouble& v2){
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

	inline vecDouble VecDivide(vecDouble& v1, vecDouble& v2){
		assert(v1.size() == v2.size());
		vecDouble result(v1.size());
		vecDouble::iterator it1 = v1.begin();
		vecDouble::iterator it2 = v2.begin();
		vecDouble::iterator res = result.begin();
		for (; it1 != v1.end(); it1++, it2++, res++){
			(*res) = (*it1) / (*it2);
		}
		return result;
	}

	inline double VecDistance(vecDouble& primary, vecDouble& secondary){
		assert(primary.size() == secondary.size());
		vecDouble diffVec(primary.size());

		vecDouble::iterator dif = diffVec.begin();
		vecDouble::iterator pri = primary.begin();	
		vecDouble::iterator sec = secondary.begin();

		for(; dif != diffVec.end(); dif++, pri++, sec++){
			(*dif) = (*pri) - (*sec);
		}
		double retVal = ublas::norm_2(diffVec);
		return retVal;
	}

	inline void FillVec(vecDouble& vector, double value){
		vecDouble::iterator it = vector.begin();
		for (; it != vector.end(); it++){
			(*it) = value;
		}
	}

	inline int Sign(double& num){
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

	inline vecDouble Error(vecDouble& first, vecDouble& second){
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

	inline double Error(double& first, double& second){
		double result = 0;
		if (first != UNKNOWN && second != UNKNOWN){
			result = first - second;
		}
		return result;
	}

	inline vecDouble FilterError(vecDouble& errorVec, ints& primaryIndexes){
		vecDouble result(errorVec.size());
		FillVec(result, 0.0);
		if (primaryIndexes.size() > 0){
			for (unsigned int i = 0; i < primaryIndexes.size(); i++){
				result[primaryIndexes[i]] = errorVec[primaryIndexes[i]];
			}
		}
		else {
			result = errorVec;
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
				FillVec(storage[name], 0.0);
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
		typename T::iterator it = inVector.begin();
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

	//This is fine for small vectors but would probably not be ideal for large vectors. 
	//Though the only way to get faster would be to use a sorted vector.
	template<class T, class V>
	inline bool InVector(T &inVector, V& inItem){
		bool retBool = false;
		BOOST_FOREACH(V item, inVector){
			if (item == inItem){
				retBool = true;
			}
		}
		return retBool;
	}

	class RandomGenerator01 : public StaticRange {
	public:
		RandomGenerator01() : dist(), randgen(gen,dist) {}
		double operator ()() {
			return randgen();
		}
	private:
		UniformDist01 dist;
		RandGenerator01 randgen;
	};

	class RandomGeneratorInt : public StaticRange {
	public:
		RandomGeneratorInt(long start, long end) : dist(start, end), randgen(gen,dist){}
		long operator ()() {
			return randgen();
		}
	private:
		UniformDistInt dist;
		RandGeneratorInt randgen;
	};

	inline void printDoubles(doubles toPrint, std::iostream& stream){
		BOOST_FOREACH(double db, toPrint){
			stream << db << "\t";
		}
	}

	inline strings splitString(string inString, string charSep){
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep(charSep.c_str());
		tokenizer tok(inString, sep);
		strings retStrings;
		BOOST_FOREACH(string token, tok){
			retStrings.push_back(token);
		}
		return retStrings;
	}

}

#endif