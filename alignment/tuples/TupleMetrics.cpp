#include <alignment/tuples/TupleMask.h>
#include <alignment/tuples/TupleMetrics.hpp>

#include <cstdint>

TupleMetrics::TupleMetrics() : tupleSize(0), tupleMask(0) {}

void TupleMetrics::InitializeMask() { tupleMask = TupleMask[tupleSize]; }

void TupleMetrics::Initialize(int pTupleSize)
{
    tupleSize = pTupleSize;
    InitializeMask();
}
