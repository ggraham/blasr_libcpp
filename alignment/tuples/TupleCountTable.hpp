#ifndef _BLASR_TUPLE_COUNT_TABLE_HPP_
#define _BLASR_TUPLE_COUNT_TABLE_HPP_

#include <cassert>
#include <algorithm>
#include <fstream>
#include <iostream>

#include "TupleMetrics.hpp"

template <typename TSequence, typename TTuple>
class TupleCountTable
{
public:
    int *countTable;
    TupleData countTableLength;
    int nTuples;
    TupleMetrics tm;
    bool deleteStructures;
    void InitCountTable(TupleMetrics &ptm);

    TupleCountTable();
    ~TupleCountTable();
    void Free();

    void IncrementCount(TTuple &tuple);
    void AddSequenceTupleCountsLR(TSequence &seq);
    void Write(std::ofstream &out);
    void Read(std::ifstream &in);
};

#include "TupleCountTableImpl.hpp"

#endif
