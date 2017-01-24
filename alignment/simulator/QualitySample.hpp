#ifndef _SIMULATOR_QUALITY_SAMPLE_HPP_
#define _SIMULATOR_QUALITY_SAMPLE_HPP_

#include <iostream>
#include "../../pbdata/SMRTSequence.hpp"
#include "../../pbdata/Types.h"
#include "../../pbdata/qvs/QualityValue.hpp"

#define NQV 4
#define NFV 3
#define NT 2

class QualitySample
{
public:
    QualityValue qv[NQV];
    HalfWord frameValues[NFV];
    Nucleotide tags[NT];

    void CopyFromSequence(SMRTSequence &seq, int pos);

    void Write(std::ofstream &out);

    void Read(std::ifstream &in);

    void CreateFromRead(SMRTSequence &seq, DNALength pos);
};

#endif
