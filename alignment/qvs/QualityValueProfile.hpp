#ifndef _BLASR_QUALITY_VALUE_PROFILE_HPP_
#define _BLASR_QUALITY_VALUE_PROFILE_HPP_

#include "../tuples/DNATuple.hpp"
#include "../tuples/TupleMetrics.hpp"
#include <fstream>
#include "../../pbdata/qvs/QualityValue.hpp"
#include "../../pbdata/matrix/FlatMatrix.hpp"

class QualityValueProfile {
	int wordSize;
	int numQualityValues;
	FlatMatrix2D<int> profile;
	int nWords;
	TupleMetrics tm;
public:
	static const int CDF_GRANULARITY = 10000;
	QualityValueProfile(int _wordSize, int _numQualityValues);
	void Update(Nucleotide *seq, QualityValue qv);
   	void Print(std::ofstream &out);
   	void ProfileToCDF();
};
#endif
