#ifndef _BLASR_FORMAT_BAMPRINTER_HPP_
#define _BLASR_FORMAT_BAMPRINTER_HPP_

#ifdef USE_PBBAM

#include <cstdint>
#include <sstream>

#include <pbbam/BamHeader.h>
#include <pbbam/BamWriter.h>

#include <alignment/format/SAMPrinter.hpp>

template <typename T_Sequence>
void AlignmentToBamRecord(T_AlignmentCandidate &alignment, T_Sequence &read, T_Sequence &subread,
                          PacBio::BAM::BamRecord &bamRecord, AlignmentContext &context,
                          SupplementalQVList &qvList, Clipping clipping,
                          bool cigarUseSeqMatch = false, const bool allowAdjacentIndels = true);

namespace BAMOutput {

template <typename T_Sequence>
void PrintAlignment(T_AlignmentCandidate &alignment, T_Sequence &read, T_Sequence &subread,
                    PacBio::BAM::IRecordWriter &bamWriter, AlignmentContext &context,
                    SupplementalQVList &qvList, Clipping clipping, bool cigarUseSeqMatch = false,
                    const bool allowAdjacentIndels = true);
}

#include "BAMPrinterImpl.hpp"

#endif
#endif
