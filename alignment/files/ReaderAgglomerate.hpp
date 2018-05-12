#ifndef _BLASR_READER_AGGLOMERATE_HPP_
#define _BLASR_READER_AGGLOMERATE_HPP_

#include <cstdlib>

#include <pbdata/Enumerations.h>
#include <alignment/files/BaseSequenceIO.hpp>
#include <hdf/HDFBasReader.hpp>
#include <hdf/HDFCCSReader.hpp>
#include <pbdata/CCSSequence.hpp>
#include <pbdata/FASTAReader.hpp>
#include <pbdata/FASTQReader.hpp>
#include <pbdata/SMRTSequence.hpp>
#include <pbdata/StringUtils.hpp>
#include <pbdata/reads/ReadType.hpp>

#ifdef USE_PBBAM

#include <pbbam/BamRecord.h>
#include <pbbam/DataSet.h>
#include <pbbam/EntireFileQuery.h>
#include <pbbam/PbiFilter.h>
#include <pbbam/PbiFilterQuery.h>
// the following added to support Polymerase read for unrolled mode
#include <pbbam/virtual/ZmwReadStitcher.h>  // new interface

#include <alignment/query/PbiFilterZmwGroupQuery.h>
#include <alignment/query/SequentialZmwGroupQuery.h>

#endif

class ReaderAgglomerate : public BaseSequenceIO
{
    FASTAReader fastaReader;
    FASTQReader fastqReader;
    int readQuality;
    int stride;
    int start;
    float subsample;
    bool useRegionTable;
    bool ignoreCCS;
    ReadType::ReadTypeEnum readType;
    bool unrolled;  // indicate if unrolled mode; needed because GetNext() must know about the mode
    bool polymerase;
    std::string scrapsFileName;  // Needed for unrolled to initiate if in PBBAM

public:
    //
    // Create interfaces for reading hdf
    //
    T_HDFBasReader<SMRTSequence> hdfBasReader;
    HDFCCSReader<CCSSequence> hdfCcsReader;
    std::vector<SMRTSequence> readBuffer;
    std::vector<CCSSequence> ccsBuffer;
    std::string readGroupId;

public:
    void SetToUpper();

    void InitializeParameters();
    ReaderAgglomerate();

    ReaderAgglomerate(float _subsample);

    ReaderAgglomerate(int _stride);

    ReaderAgglomerate(int _start, int _stride);

    void GetMovieName(std::string &movieName);

    /// Get BindingKit, SequencingKit and Base Caller Version from h5.
    ///
    /// /param [out] sequencingKit - sequencingKit from
    /// /ScanData/RunInfo/SequencingKit.
    ///
    /// /param [out] bindingKit - BindingKit from
    /// /ScanData/RunInfo/BindingKit.
    ///
    /// /param [out] baseCallerVersion - Base Caller Version
    /// from /PulseData/BaseCalls/ChangeListID.
    ///
    void GetChemistryTriple(std::string &bindingKit, std::string &sequencingKit,
                            std::string &baseCallerVersion);

    bool FileHasZMWInformation();

    void SkipReadQuality();

    void IgnoreCCS();

    void UseCCS();

    int Initialize(std::string &pFileName);

    bool SetReadFileName(std::string &pFileName);

    void SetScrapsFileName(std::string &pFileName);  // needed for unrolled

    int Initialize(FileType &pFileType, std::string &pFileName);

    bool HasRegionTable();

    // add unrolled mode, to indicate we need to initialize VP/VPC|Reader
    // polymerase mode will only work with BAM records
    int Initialize(bool unrolled_mode = false, bool polymerase_mode = false);

    ReaderAgglomerate &operator=(ReaderAgglomerate &rhs);

    bool Subsample(float rate);

    // Set read type to SUBREAD, CCS, or UNKNOWN.
    void SetReadType(const ReadType::ReadTypeEnum &readType_);

    // returns read type, SUBREAD, CCS, or UNKNOWN
    ReadType::ReadTypeEnum GetReadType();

public:
    int GetNext(FASTASequence &seq);
    int GetNext(FASTQSequence &seq);
    int GetNext(SMRTSequence &seq);
    int GetNext(CCSSequence &seq);
    int GetNext(std::vector<SMRTSequence> &reads);

    template <typename T_Sequence>
    int GetNext(T_Sequence &seq, int &randNum);

    int GetNextBases(SMRTSequence &seq, bool readQVs);

    int Advance(int nSteps);

    void Close();

#ifdef USE_PBBAM
public:
    // Define reader to fetch sequences from bam.
    PacBio::BAM::DataSet *dataSetPtr;
    PacBio::BAM::EntireFileQuery *entireFileQueryPtr;
    PacBio::BAM::EntireFileQuery::iterator entireFileIterator;
    PacBio::BAM::PbiFilterQuery *pbiFilterQueryPtr;
    PacBio::BAM::PbiFilterQuery::iterator pbiFilterIterator;
    PacBio::BAM::SequentialZmwGroupQuery *sequentialZmwQueryPtr;
    PacBio::BAM::SequentialZmwGroupQuery::iterator sequentialZmwIterator;
    PacBio::BAM::PbiFilterZmwGroupQuery *pbiFilterZmwQueryPtr;
    PacBio::BAM::PbiFilterZmwGroupQuery::iterator pbiFilterZmwIterator;
    // the following to added to support ZMW reads in unrolled mode
    PacBio::BAM::ZmwReadStitcher *VPReader;  // new interface
#endif
};

template <typename T_Sequence>
int ReadChunkByNReads(ReaderAgglomerate &reader, std::vector<T_Sequence> &reads, int maxNReads);

template <typename T_Sequence>
int ReadChunkBySize(ReaderAgglomerate &reader, std::vector<T_Sequence> &reads, int maxMemorySize);

#include "ReaderAgglomerateImpl.hpp"

#endif
