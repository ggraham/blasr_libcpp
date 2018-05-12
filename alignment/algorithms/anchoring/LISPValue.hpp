#ifndef _BLASR_LISPVALUE_HPP_
#define _BLASR_LISPVALUE_HPP_

#include <cmath>

#include <alignment/algorithms/anchoring/ScoreAnchors.hpp>
#include <alignment/datastructures/anchoring/MatchPos.hpp>
#include <alignment/tuples/TupleCountTable.hpp>

template <typename T_MatchPos>
void StoreNonOverlappingIndices(std::vector<T_MatchPos> &lis, std::vector<T_MatchPos> &noOvpLis);

template <typename T_TextSequence, typename T_Sequence, typename T_MatchPos, typename T_Tuple>
float ComputeLISPValue(std::vector<T_MatchPos> &lis, T_TextSequence &text, T_Sequence &read,
                       TupleMetrics &tm, TupleCountTable<T_TextSequence, T_Tuple> &ct,
                       int &lisNBases, int &lisSize);

#include "LISPValueImpl.hpp"

#endif
