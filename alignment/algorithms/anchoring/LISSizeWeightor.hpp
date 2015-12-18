#ifndef _BLASR_LIS_SIZE_WEIGHTOR_HPP_
#define _BLASR_LIS_SIZE_WEIGHTOR_HPP_

#include <Types.h> // pbdata

template<typename T_MatchList>
class LISSizeWeightor {
public:
	MatchWeight operator()(T_MatchList &matchList);
};

#include "LISSizeWeightorImpl.hpp"

#endif
