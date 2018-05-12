#ifndef _BLASR_BWT_SEARCH_HPP_
#define _BLASR_BWT_SEARCH_HPP_

#include <vector>

#include <alignment/bwt/BWT.hpp>
#include <alignment/datastructures/anchoring/AnchorParameters.hpp>
#include <alignment/datastructures/anchoring/MatchPos.hpp>
#include <pbdata/FASTASequence.hpp>

int MapReadToGenome(BWT &bwt, FASTASequence &seq, DNALength subreadStart, DNALength subreadEnd,
                    std::vector<ChainedMatchPos> &matchPosList, AnchorParameters &params,
                    int &numBasesAnchored, std::vector<DNALength> &spv,
                    std::vector<DNALength> &epv);

int MapReadToGenome(BWT &bwt, FASTASequence &seq, DNALength start, DNALength end,
                    std::vector<ChainedMatchPos> &matchPosList, AnchorParameters &params,
                    int &numBasesAnchored);

template <typename T_MappingBuffers>
int MapReadToGenome(BWT &bwt, FASTASequence &seq, std::vector<ChainedMatchPos> &matchPosList,
                    AnchorParameters &params, int &numBasesAnchored,
                    T_MappingBuffers &mappingBuffers);

#include "BWTSearchImpl.hpp"

#endif
