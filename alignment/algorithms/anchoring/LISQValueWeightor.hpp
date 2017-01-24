#ifndef _BLASR_LIS_QVALUE_WEIGHTOR_HPP_
#define _BLASR_LIS_QVALUE_WEIGHTOR_HPP_

// pbdata
#include "../../../pbdata/DNASequence.hpp"
#include "../../../pbdata/FASTQSequence.hpp"
#include "../../../pbdata/qvs/QualityValue.hpp"

template <typename T_MatchList, typename T_Sequence>
class LISQValueWeightor
{
public:
    T_Sequence *seq;
    float operator()(const T_MatchList &matchList);
};

template <typename T_MatchList, typename T_Sequence>
float LISQValueWeightor<T_MatchList, T_Sequence>::operator()(const T_MatchList &matchList)
{
    float totalQ;
    DNALength nBases;
    VectorIndex i;
    totalQ = 0.0;
    nBases = 0;
    for (i = 0; i < matchList.size(); i++) {
        DNALength mp;
        for (mp = matchList[i].q; mp < matchList[i].q + matchList[i].w; mp++) {
            totalQ += (*seq).qual[mp];
        }
        nBases += matchList[i].w;
    }
    if (nBases > 0) {
        return totalQ / nBases;
    }
}

#endif
