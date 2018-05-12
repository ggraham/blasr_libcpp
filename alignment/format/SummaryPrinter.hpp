#ifndef SUMMARY_ALIGNMENT_PRINTER_H_
#define SUMMARY_ALIGNMENT_PRINTER_H_

#include <alignment/datastructures/alignment/AlignmentCandidate.hpp>
#include <pbdata/FASTQSequence.hpp>

namespace SummaryOutput {

void Print(AlignmentCandidate<DNASequence, FASTQSequence> &alignment, std::ostream &outFile);

void PrintHeader(std::ostream &out);
}

#endif
