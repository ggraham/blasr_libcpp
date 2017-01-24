#include "ReaderAgglomerate.hpp"

void ReaderAgglomerate::SetToUpper() { fastaReader.SetToUpper(); }

void ReaderAgglomerate::InitializeParameters()
{
    start = 0;
    stride = 1;
    subsample = 1.1;
    readQuality = 1;
    useRegionTable = true;
    ignoreCCS = true;
    readType = ReadType::SUBREAD;
    unrolled = false;  // indicate unrolled mode , needed by GetNext
#ifdef USE_PBBAM
    dataSetPtr = nullptr;
    entireFileQueryPtr = nullptr;
    pbiFilterQueryPtr = nullptr;
    sequentialZmwQueryPtr = nullptr;
    pbiFilterZmwQueryPtr = nullptr;
    // the following two for unrolling
    VPReader = nullptr;   // for PBBAM
    VPCReader = nullptr;  // for PBDATASET
#endif
}

// Constructors

ReaderAgglomerate::ReaderAgglomerate() { InitializeParameters(); }

ReaderAgglomerate::ReaderAgglomerate(float _subsample)
{
    this->InitializeParameters();
    subsample = _subsample;
}

ReaderAgglomerate::ReaderAgglomerate(int _stride)
{
    this->InitializeParameters();
    stride = _stride;
}

ReaderAgglomerate::ReaderAgglomerate(int _start, int _stride)
{
    this->InitializeParameters();
    start = _start;
    stride = _stride;
}

// End of Constructors

void ReaderAgglomerate::GetMovieName(string &movieName)
{
    if (fileType == FileType::Fasta || fileType == FileType::Fastq) {
        movieName = fileName;
    } else if (fileType == FileType::HDFPulse || fileType == FileType::HDFBase) {
        movieName = hdfBasReader.GetMovieName();
    } else if (fileType == FileType::HDFCCS || fileType == FileType::HDFCCSONLY) {
        movieName = hdfCcsReader.GetMovieName();
    } else if (fileType == FileType::PBBAM || fileType == FileType::PBDATASET) {
#ifdef USE_PBBAM
        assert("Reading movie name from BAM using ReaderAgglomerate is not supported." == 0);
#endif
    }
}

void ReaderAgglomerate::GetChemistryTriple(string &bindingKit, string &sequencingKit,
                                           string &baseCallerVersion)
{
    if (fileType == FileType::HDFPulse || fileType == FileType::HDFBase) {
        hdfBasReader.GetChemistryTriple(bindingKit, sequencingKit, baseCallerVersion);
    } else if (fileType == FileType::HDFCCS || fileType == FileType::HDFCCSONLY) {
        hdfCcsReader.GetChemistryTriple(bindingKit, sequencingKit, baseCallerVersion);
    } else if (fileType == FileType::PBBAM || fileType == FileType::PBDATASET) {
#ifdef USE_PBBAM
        assert("Reading chemistry triple from BAM using ReaderAgglomerate is not supported." == 0);
#endif
    } else {
        sequencingKit = bindingKit = baseCallerVersion = "";
    }
}

void ReaderAgglomerate::SetReadType(const ReadType::ReadTypeEnum &readType_)
{
    readType = readType_;
}

ReadType::ReadTypeEnum ReaderAgglomerate::GetReadType() { return readType; }

bool ReaderAgglomerate::FileHasZMWInformation()
{
    return (fileType == FileType::HDFPulse || fileType == FileType::HDFBase ||
            fileType == FileType::HDFCCS || fileType == FileType::HDFCCSONLY);
}

void ReaderAgglomerate::SkipReadQuality() { readQuality = 0; }

void ReaderAgglomerate::IgnoreCCS() { ignoreCCS = true; }

void ReaderAgglomerate::UseCCS()
{
    ignoreCCS = false;
    hdfBasReader.SetReadBasesFromCCS();
}

void ReaderAgglomerate::SetScrapsFileName(string &pFileName) { scrapsFileName = pFileName; }

bool ReaderAgglomerate::SetReadFileName(string &pFileName)
{
    if (DetermineFileTypeByExtension(pFileName, fileType)) {
        fileName = pFileName;
        return true;
    } else {
        return false;
    }
}

int ReaderAgglomerate::Initialize(string &pFileName)
{
    if (DetermineFileTypeByExtension(pFileName, fileType)) {
        fileName = pFileName;
        return Initialize();
    }
    return false;
}

int ReaderAgglomerate::Initialize(FileType &pFileType, string &pFileName)
{
    SetFiles(pFileType, pFileName);
    return Initialize();
}

#define UNREACHABLE()                                                                 \
    cout << "ERROR! Hit unreachable code in " << __FILE__ << ':' << __LINE__ << endl; \
    assert(0)

bool ReaderAgglomerate::HasRegionTable()
{
    switch (fileType) {
        case FileType::PBBAM:
        case FileType::PBDATASET:
        case FileType::Fasta:
        case FileType::Fastq:
            return false;
            break;
        case FileType::HDFPulse:
        case FileType::HDFBase:
            return hdfBasReader.HasRegionTable();
            break;
        case FileType::HDFCCSONLY:
        case FileType::HDFCCS:
            return hdfCcsReader.HasRegionTable();
            break;
        case FileType::Fourbit:
        case FileType::None:
            UNREACHABLE();
            break;
    }
    return false;
}

#ifdef USE_PBBAM

#define GET_NEXT_FROM_BAM()                                                              \
    numRecords = 0;                                                                      \
    while (entireFileIterator != entireFileQueryPtr->end()) {                            \
        if (not SMRTSequence::IsValid(*entireFileIterator)) {                            \
            std::cerr << "Skipping an invalid read " << (*entireFileIterator).FullName() \
                      << std::endl;                                                      \
            entireFileIterator++;                                                        \
        } else {                                                                         \
            numRecords = 1;                                                              \
            seq.Copy(*entireFileIterator);                                               \
            entireFileIterator++;                                                        \
            break;                                                                       \
        }                                                                                \
    }

#define GET_NEXT_FROM_DATASET()                                                         \
    numRecords = 0;                                                                     \
    while (pbiFilterIterator != pbiFilterQueryPtr->end()) {                             \
        if (not SMRTSequence::IsValid(*pbiFilterIterator)) {                            \
            std::cerr << "Skipping an invalid read " << (*pbiFilterIterator).FullName() \
                      << std::endl;                                                     \
            pbiFilterIterator++;                                                        \
        } else {                                                                        \
            numRecords = 1;                                                             \
            seq.Copy(*pbiFilterIterator);                                               \
            pbiFilterIterator++;                                                        \
            break;                                                                      \
        }                                                                               \
    }

#define RESET_PBBAM_PTRS()               \
    if (dataSetPtr != nullptr) {         \
        delete dataSetPtr;               \
        dataSetPtr = nullptr;            \
    }                                    \
    if (entireFileQueryPtr) {            \
        delete entireFileQueryPtr;       \
        entireFileQueryPtr = nullptr;    \
    }                                    \
    if (pbiFilterQueryPtr) {             \
        delete pbiFilterQueryPtr;        \
        pbiFilterQueryPtr = nullptr;     \
    }                                    \
    if (sequentialZmwQueryPtr) {         \
        delete sequentialZmwQueryPtr;    \
        sequentialZmwQueryPtr = nullptr; \
    }                                    \
    if (pbiFilterZmwQueryPtr) {          \
        delete pbiFilterZmwQueryPtr;     \
        pbiFilterZmwQueryPtr = nullptr;  \
    }                                    \
    if (VPReader) {                      \
        delete VPReader;                 \
        VPReader = nullptr;              \
    }                                    \
    if (VPCReader) {                     \
        delete VPCReader;                \
        VPCReader = nullptr;             \
    }
#endif

int ReaderAgglomerate::Initialize(bool unrolled_mode)
{
    int init = 1;
    switch (fileType) {
        case FileType::Fasta:
            init = fastaReader.Init(fileName);
            break;
        case FileType::Fastq:
            init = fastqReader.Init(fileName);
            break;
        case FileType::HDFCCSONLY:
            ignoreCCS = false;
            hdfCcsReader.SetReadBasesFromCCS();
            hdfCcsReader.InitializeDefaultIncludedFields();
            init = hdfCcsReader.Initialize(fileName);
            if (init == 0) return 0;
            break;
        case FileType::HDFPulse:
        case FileType::HDFBase:
            //
            // Here one needs to test and see if the hdf file contains ccs.
            // If this is the case, then the file type is HDFCCS.
            if (hdfCcsReader.BasFileHasCCS(fileName) and !ignoreCCS) {
                fileType = FileType::HDFCCS;
                hdfCcsReader.InitializeDefaultIncludedFields();
                init = hdfCcsReader.Initialize(fileName);
                if (init == 0) return 0;
            } else {
                hdfBasReader.InitializeDefaultIncludedFields();
                init = hdfBasReader.Initialize(fileName);
                //
                // This code is added so that meaningful names are printed
                // when running on simulated data that contains the coordinate
                // information.
                if (init == 0) return 0;
            }
            break;
        case FileType::PBBAM:
        case FileType::PBDATASET:
#ifdef USE_PBBAM
            RESET_PBBAM_PTRS();
            unrolled = unrolled_mode;
            // create dataset , common for both unrolled and regular modes
            try {
                dataSetPtr = new PacBio::BAM::DataSet(fileName);
            } catch (std::exception e) {
                cout << "ERROR! Failed to open " << fileName << ": " << e.what() << endl;
                return 0;
            }
            if (unrolled) {
                if (fileType == FileType::PBBAM) {
                    // Handle PBBAM here , use scrapFileName
                    VPReader = new PacBio::BAM::ZmwReadStitcher(fileName, scrapsFileName);
                    assert(VPReader != nullptr);
                    //
                } else if (fileType == FileType::PBDATASET) {
                    dataSetPtr = new PacBio::BAM::DataSet(fileName);
                    // No need in setting filters for PolymeraseReads
                    // prefiltering, in a form it is currently implemented migght crate Polymerase reads
                    // with skipped subreads, which defies the whole purpose of unrolled mode
                    VPCReader = new PacBio::BAM::ZmwReadStitcher(*dataSetPtr);
                    assert(VPCReader != nullptr);
                }
            } else {
                if (fileType == FileType::PBBAM) {
                    entireFileQueryPtr = new PacBio::BAM::EntireFileQuery(*dataSetPtr);
                    assert(entireFileQueryPtr != nullptr);
                    entireFileIterator = entireFileQueryPtr->begin();

                    sequentialZmwQueryPtr = new PacBio::BAM::SequentialZmwGroupQuery(*dataSetPtr);
                    assert(sequentialZmwQueryPtr != nullptr);
                    sequentialZmwIterator = sequentialZmwQueryPtr->begin();
                } else if (fileType == FileType::PBDATASET) {
                    const PacBio::BAM::PbiFilter filter =
                        PacBio::BAM::PbiFilter::FromDataSet(*dataSetPtr);
                    pbiFilterQueryPtr = new PacBio::BAM::PbiFilterQuery(filter, *dataSetPtr);
                    assert(pbiFilterQueryPtr != nullptr);
                    pbiFilterIterator = pbiFilterQueryPtr->begin();

                    pbiFilterZmwQueryPtr =
                        new PacBio::BAM::PbiFilterZmwGroupQuery(filter, *dataSetPtr);
                    assert(pbiFilterZmwQueryPtr != nullptr);
                    pbiFilterZmwIterator = pbiFilterZmwQueryPtr->begin();
                }
            }
            break;
#endif
        case FileType::HDFCCS:
        case FileType::Fourbit:
        case FileType::None:
            UNREACHABLE();
            break;
    }
    readGroupId = "";
    if (init == 0 || (start > 0 && Advance(start) == 0)) {
        return 0;
    };
    if (fileType != FileType::PBBAM and fileType != FileType::PBDATASET) {
        // All reads from a non-PBBAM file must have the same read group id.
        // Reads from a PABBAM file may come from different read groups.
        // We have sync reader.readGroupId and SMRTSequence.readGroupId everytime
        // GetNext() is called.
        string movieName;
        GetMovieName(movieName);
        readGroupId = MakeReadGroupId(movieName, readType);
    }
    return 1;
}

ReaderAgglomerate &ReaderAgglomerate::operator=(ReaderAgglomerate &rhs)
{
    fileType = rhs.fileType;
    fileName = rhs.fileName;
    return *this;
}

bool ReaderAgglomerate::Subsample(float rate)
{
    bool retVal = true;
    while ((rand() % 100 + 1) > (rate * 100) and (retVal = Advance(1)))
        ;
    return retVal;
}

int ReaderAgglomerate::GetNext(FASTASequence &seq)
{
    int numRecords = 0;
    if (Subsample(subsample) == 0) {
        return 0;
    }
    switch (fileType) {
        case FileType::Fasta:
            numRecords = fastaReader.GetNext(seq);
            break;
        case FileType::Fastq:
            numRecords = fastqReader.GetNext(seq);
            break;
        case FileType::HDFPulse:
        case FileType::HDFBase:
            numRecords = hdfBasReader.GetNext(seq);
            break;
        case FileType::HDFCCSONLY:
        case FileType::HDFCCS:
            cout << "ERROR! Reading CCS into a structure that cannot handle it." << endl;
            assert(0);
            break;
        case FileType::PBDATASET:
#ifdef USE_PBBAM
            GET_NEXT_FROM_DATASET();
            break;
#endif
        case FileType::PBBAM:
#ifdef USE_PBBAM
            GET_NEXT_FROM_BAM();
            break;
#endif
        case FileType::Fourbit:
        case FileType::None:
            UNREACHABLE();
            break;
    }
    seq.CleanupOnFree();
    return numRecords;
}

int ReaderAgglomerate::GetNext(FASTQSequence &seq)
{
    int numRecords = 0;
    if (Subsample(subsample) == 0) {
        return 0;
    }
    switch (fileType) {
        case FileType::Fasta:
            numRecords = fastaReader.GetNext(seq);
            break;
        case FileType::Fastq:
            numRecords = fastqReader.GetNext(seq);
            break;
        case FileType::HDFPulse:
        case FileType::HDFBase:
            numRecords = hdfBasReader.GetNext(seq);
            break;
        case FileType::PBDATASET:
#ifdef USE_PBBAM
            GET_NEXT_FROM_DATASET();
            break;
#endif
        case FileType::PBBAM:
#ifdef USE_PBBAM
            GET_NEXT_FROM_BAM();
            break;
#endif
        case FileType::HDFCCSONLY:
        case FileType::HDFCCS:
            cout << "ERROR! Reading CCS into a structure that cannot handle it." << endl;
            assert(0);
            break;
        case FileType::Fourbit:
        case FileType::None:
            UNREACHABLE();
            break;
    }
    if (stride > 1) Advance(stride - 1);
    return numRecords;
}

int ReaderAgglomerate::GetNext(vector<SMRTSequence> &reads)
{
    int numRecords = 0;
    reads.clear();

    if (Subsample(subsample) == 0) {
        return 0;
    }
    if (fileType == FileType::PBBAM) {
#ifdef USE_PBBAM
        // no need to check for unrolled mode, vector of SMRTS is being received
        while (sequentialZmwIterator != sequentialZmwQueryPtr->end()) {
            const vector<PacBio::BAM::BamRecord> &records = *sequentialZmwIterator;
            // bug 30566, short term solution, ignore bad record.
            bool OK = true;
            for (size_t i = 0; i < records.size(); i++) {
                if (not SMRTSequence::IsValid(records[i])) {
                    OK = false;
                    std::cerr << "Skipping all subreads in " << records[i].MovieName() << "/"
                              << records[i].HoleNumber() << ", because " << records[i].FullName()
                              << " is invalid." << std::endl;
                    break;
                }
            }
            if (OK) {
                numRecords = records.size();
                reads.resize(numRecords);
                for (size_t i = 0; i < records.size(); i++) {
                    reads[i].Copy(records[i]);
                }
                sequentialZmwIterator++;
                break;
            } else {
                sequentialZmwIterator++;
            }
        }
#endif
    } else if (fileType == FileType::PBDATASET) {
#ifdef USE_PBBAM
        // no need to check for unrolled mode, vector of SMRTS is being received
        while (pbiFilterZmwIterator != pbiFilterZmwQueryPtr->end()) {
            const vector<PacBio::BAM::BamRecord> &records = *pbiFilterZmwIterator;
            bool OK = true;
            for (size_t i = 0; i < records.size(); i++) {
                if (not SMRTSequence::IsValid(records[i])) {
                    OK = false;
                    std::cerr << "Skipping all subreads in " << records[i].MovieName() << "/"
                              << records[i].HoleNumber() << ", because " << records[i].FullName()
                              << " is invalid." << std::endl;
                    break;
                }
            }
            if (OK) {
                numRecords = records.size();
                reads.resize(numRecords);
                for (size_t i = 0; i < records.size(); i++) {
                    reads[i].Copy(records[i]);
                }
                pbiFilterZmwIterator++;
                break;
            } else {
                pbiFilterZmwIterator++;
            }
        }
#endif
    } else {
        UNREACHABLE();
    }
    if (numRecords >= 1) readGroupId = reads[0].ReadGroupId();
    return numRecords;
}

// for now the only one which might be in unrolled mode: obtains SMRTSequence scalar
int ReaderAgglomerate::GetNext(SMRTSequence &seq)
{
    int numRecords = 0;

    if (Subsample(subsample) == 0) {
        return 0;
    }
    switch (fileType) {
        case FileType::Fasta:
            numRecords = fastaReader.GetNext(seq);
            break;
        case FileType::Fastq:
            numRecords = fastqReader.GetNext(seq);
            break;
        case FileType::HDFPulse:
        case FileType::HDFBase:
            numRecords = hdfBasReader.GetNext(seq);
            break;
        case FileType::HDFCCSONLY:
            cout << "ERROR! Reading CCS into a structure that cannot handle it." << endl;
            assert(0);
            break;
        case FileType::HDFCCS:
            assert(ignoreCCS == false);
            assert(hdfBasReader.readBasesFromCCS == true);
            numRecords = hdfBasReader.GetNext(seq);
            break;
        case FileType::PBDATASET:
#ifdef USE_PBBAM
            if (unrolled) {
                if (VPCReader->HasNext()) {
                    // TODO check for length mismatch (as temporary fix)

                    PacBio::BAM::VirtualZmwBamRecord record = VPCReader->Next();

                    numRecords = 1;    // a single record only
                    seq.Copy(record);  // need to copy into seq
                    // denote, no iterator so no need to advance anything. HasNext advances to ath next VPBR
                }
            } else {
                GET_NEXT_FROM_DATASET();
            }
            break;
#endif
        case FileType::PBBAM:
#ifdef USE_PBBAM
            // TODO unrolled
            if (unrolled) {
                if (VPReader->HasNext()) {
                    // TODO check for length mismatch (as temporary fix)

                    PacBio::BAM::VirtualZmwBamRecord record = VPReader->Next();

                    numRecords = 1;    // a single record only
                    seq.Copy(record);  // need to copy into seq
                    // denote, no iterator so no need to advance anything. HasNext advances to ath next VPBR
                }
            } else {
                GET_NEXT_FROM_BAM();
            }
            break;
#endif
        case FileType::Fourbit:
        case FileType::None:
            UNREACHABLE();
            break;
    }
    // A sequence read from a Non-BAM files does not have read group id
    // and should be empty, use this->readGroupId instead. Otherwise,
    // read group id should be loaded from BamRecord to SMRTSequence,
    // update this->readGroupId accordingly.
    if (fileType != FileType::PBBAM and fileType != FileType::PBDATASET)
        seq.ReadGroupId(readGroupId);
    else
        readGroupId = seq.ReadGroupId();

    if (stride > 1) Advance(stride - 1);
    return numRecords;
}

int ReaderAgglomerate::GetNextBases(SMRTSequence &seq, bool readQVs)
{
    int numRecords = 0;

    if (Subsample(subsample) == 0) {
        return 0;
    }
    switch (fileType) {
        case FileType::Fasta:
            cout << "ERROR! Can not GetNextBases from a Fasta File." << endl;
            assert(0);
            break;
        case FileType::Fastq:
            cout << "ERROR! Can not GetNextBases from a Fastq File." << endl;
            assert(0);
            break;
        case FileType::HDFPulse:
        case FileType::HDFBase:
            numRecords = hdfBasReader.GetNextBases(seq, readQVs);
            break;
        case FileType::HDFCCSONLY:
            cout << "ERROR! Reading CCS into a structure that cannot handle it." << endl;
            assert(0);
            break;
        case FileType::HDFCCS:
            cout << "ERROR! Can not GetNextBases from a CCS File." << endl;
            assert(0);
            break;
        case FileType::PBBAM:
        case FileType::PBDATASET:
#ifdef USE_PBBAM
            cout << "ERROR! Can not GetNextBases from a BAM File." << endl;
#endif
        case FileType::Fourbit:
        case FileType::None:
            UNREACHABLE();
            break;
    }

    if (fileType != FileType::PBBAM and fileType != FileType::PBDATASET)
        seq.ReadGroupId(readGroupId);
    else
        readGroupId = seq.ReadGroupId();

    if (stride > 1) Advance(stride - 1);
    return numRecords;
}

int ReaderAgglomerate::GetNext(CCSSequence &seq)
{
    int numRecords = 0;
    if (Subsample(subsample) == 0) {
        return 0;
    }

    switch (fileType) {
        case FileType::Fasta:
            // This just reads in the fasta sequence as if it were a ccs sequence
            numRecords = fastaReader.GetNext(seq);
            seq.SubreadStart(0).SubreadEnd(0);
            break;
        case FileType::Fastq:
            numRecords = fastqReader.GetNext(seq);
            seq.SubreadStart(0).SubreadEnd(0);
            break;
        case FileType::HDFPulse:
        case FileType::HDFBase:
            numRecords = hdfBasReader.GetNext(seq);
            break;
        case FileType::HDFCCSONLY:
        case FileType::HDFCCS:
            numRecords = hdfCcsReader.GetNext(seq);
            break;
        case FileType::PBBAM:
        case FileType::PBDATASET:
#ifdef USE_PBBAM
            cout << "ERROR! Could not read BamRecord as CCSSequence" << endl;
#endif
        case FileType::Fourbit:
        case FileType::None:
            UNREACHABLE();
            break;
    }

    if (fileType != FileType::PBBAM and fileType != FileType::PBDATASET)
        seq.ReadGroupId(readGroupId);
    else
        readGroupId = seq.ReadGroupId();

    if (stride > 1) Advance(stride - 1);
    return numRecords;
}

int ReaderAgglomerate::Advance(int nSteps)
{
    switch (fileType) {
        case FileType::Fasta:
            return fastaReader.Advance(nSteps);
        case FileType::HDFPulse:
        case FileType::HDFBase:
            return hdfBasReader.Advance(nSteps);
        case FileType::HDFCCSONLY:
        case FileType::HDFCCS:
            return hdfCcsReader.Advance(nSteps);
        case FileType::Fastq:
            return fastqReader.Advance(nSteps);
        case FileType::PBBAM:
        case FileType::PBDATASET:
        case FileType::Fourbit:
        case FileType::None:
            UNREACHABLE();
            break;
    }
    return false;
}

void ReaderAgglomerate::Close()
{
    switch (fileType) {
        case FileType::Fasta:
            fastaReader.Close();
            break;
        case FileType::Fastq:
            fastqReader.Close();
            break;
        case FileType::HDFPulse:
        case FileType::HDFBase:
            hdfBasReader.Close();
            break;
        case FileType::HDFCCSONLY:
        case FileType::HDFCCS:
            hdfCcsReader.Close();
            break;
        case FileType::PBBAM:
        case FileType::PBDATASET:
#ifdef USE_PBBAM
            RESET_PBBAM_PTRS();
            break;
#endif
        case FileType::Fourbit:
        case FileType::None:
            UNREACHABLE();
            break;
    }
}
