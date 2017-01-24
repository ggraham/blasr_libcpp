#ifndef _BLASR_LIS_SIZE_WEIGHTOR_HPP_
#define _BLASR_LIS_SIZE_WEIGHTOR_HPP_

#include "../../../pbdata/Types.h"

template <typename T_MatchList>
class LISSizeWeightor
{
public:
    MatchWeight operator()(T_MatchList &matchList);
};

#include "LISSizeWeightorImpl.hpp"

#endif
