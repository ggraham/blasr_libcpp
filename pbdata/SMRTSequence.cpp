// Author: Mark Chaisson

#include "SMRTSequence.hpp"
#include <cstdlib>
#include "utils/SMRTTitle.hpp"

SMRTSequence::SMRTSequence()
    : FASTQSequence()
    , subreadStart_(0)  // subread start
    , subreadEnd_(0)    // subread end
    , readGroupId_("")  // read group id
    , zmwData(ZMWGroupEntry())
    , lowQualityPrefix(0)        // By default, allow the entire read.
    , lowQualitySuffix(0)        // By default, allow the entire read.
    , highQualityRegionScore(0)  // HQ read score
    , readScore(0)               // read score
    , copiedFromBam(false)
    , preBaseFrames(nullptr)
    , widthInFrames(nullptr)
    , meanSignal(nullptr)    // not allocated by default
    , maxSignal(nullptr)     // not allocated by default
    , midSignal(nullptr)     // not allocated by default
    , classifierQV(nullptr)  // not allocated by default
    , startFrame(nullptr)    // not allocated by default
    , pulseIndex(nullptr)    // not allocated by default
#ifdef USE_PBBAM
    , bamRecord(PacBio::BAM::BamRecord())
#endif
{
    // ZMWMetrics
    for (size_t i = 0; i < 4; i++) {
        hqRegionSnr_[i] = -1;
    }
}

void SMRTSequence::Allocate(DNALength length)
{
    // Assert *this has no allocated space.
    if (not(seq == NULL && preBaseFrames == NULL && widthInFrames == NULL and pulseIndex == NULL)) {
        std::cout << "ERROR, trying to double-allocate memory for a SMRTSequence." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    FASTQSequence::AllocateQualitySpace(length);
    FASTQSequence::AllocateRichQualityValues(length);
    seq = ProtectedNew<Nucleotide>(length);
    this->length = length;
    preBaseFrames = ProtectedNew<HalfWord>(length);
    widthInFrames = ProtectedNew<HalfWord>(length);
    pulseIndex = ProtectedNew<int>(length);
    subreadEnd_ = length;
    deleteOnExit = true;
}

void SMRTSequence::CompactAllocate(const DNALength length, const bool hasInsertionDeletionQVTag,
                                   const bool hasSubstitutionQVTag)
{
    // Only allocate necessary QVs for computing alignments
    // Insertion QV, Deletion QV and Deletion Tag must be either all exist or none exist
    // Substitution QV and tag must be either both exist or non exist.
    assert(seq == NULL && preBaseFrames == NULL && widthInFrames == NULL and pulseIndex == NULL);
    seq = ProtectedNew<Nucleotide>(length);
    if (hasInsertionDeletionQVTag) {
        this->AllocateInsertionQVSpace(length);
        this->insertionQV.Fill(0);
        this->AllocateDeletionQVSpace(length);
        this->deletionQV.Fill(0);
        this->AllocateDeletionTagSpace(length);
        memset(this->deletionTag, 'N', sizeof(char) * length);
    }

    if (hasSubstitutionQVTag) {
        this->AllocateSubstitutionQVSpace(length);
        this->substitutionQV.Fill(0);
        this->AllocateSubstitutionTagSpace(length);
        memset(this->substitutionTag, 'N', sizeof(char) * length);
    }
    this->length = length;
    subreadEnd_ = length;
    deleteOnExit = true;
}

void SMRTSequence::SetSubreadTitle(SMRTSequence &subread, DNALength subreadStart,
                                   DNALength subreadEnd)
{
    std::stringstream titleStream;
    titleStream << title << "/" << subreadStart << "_" << subreadEnd;
    subread.CopyTitle(titleStream.str());
}

void SMRTSequence::SetSubreadBoundaries(SMRTSequence &subread, DNALength subreadStart,
                                        DNALength subreadEnd)
{
    if (subreadEnd == static_cast<DNALength>(-1)) {
        subreadEnd = length;
    }
    assert(subreadEnd - subreadStart <= length);
    subread.subreadStart_ = subreadStart;
    subread.subreadEnd_ = subreadEnd;
    SetSubreadTitle(subread, subreadStart, subreadEnd);
}

void SMRTSequence::MakeSubreadAsMasked(SMRTSequence &subread, DNALength subreadStart,
                                       int subreadEnd)
{
    subread.Free();
    //
    // This creates the entire subread, but masks out the portions
    // that do not correspond to this insert.
    //
    static_cast<SMRTSequence *>(&subread)->Copy(*this);
    SetSubreadBoundaries(subread, subreadStart, subreadEnd);
    DNALength pos;
    for (pos = 0; pos < subreadStart; pos++) {
        subread.seq[pos] = 'N';
    }
    for (pos = subreadEnd; pos < length; pos++) {
        subread.seq[pos] = 'N';
    }
    // This is newly allocated memory, free it on exit.
    assert(subread.deleteOnExit);
}

void SMRTSequence::MakeSubreadAsReference(SMRTSequence &subread, DNALength subreadStart,
                                          int subreadEnd)
{
    subread.Free();
    //
    // Just create a reference to a substring of this read.
    //
    static_cast<FASTQSequence *>(&subread)->ReferenceSubstring(*this, subreadStart,
                                                               subreadEnd - subreadStart);
    SetSubreadBoundaries(subread, subreadStart, subreadEnd);
    // The subread references this read, protect the memory.
    assert(not subread.deleteOnExit);
}

void SMRTSequence::Copy(const SMRTSequence &rhs) { SMRTSequence::Copy(rhs, 0, rhs.length); }

void SMRTSequence::Copy(const SMRTSequence &rhs, DNALength rhsPos, DNALength rhsLength)
{
    // Sanity check
    CheckBeforeCopyOrReference(rhs, "SMRTSequence");

    // Free this SMRTSequence before copying anything from rhs.
    SMRTSequence::Free();

    FASTQSequence subseq;
    // subseq.seq is referenced, while seq.title is not, we need to call
    // subseq.Free() to prevent memory leak.
    static_cast<FASTQSequence *>(&subseq)->ReferenceSubstring(rhs, rhsPos, rhsLength);
    static_cast<FASTQSequence *>(&subseq)->CopyTitle(rhs.title, rhs.titleLength);

    if (rhs.length == 0) {
        static_cast<FASTQSequence *>(this)->Copy(subseq);
        //
        // Make sure that no values of length 0 are allocated by returning here.
        //
    } else {
        assert(rhs.seq != seq);
        assert(rhsLength <= rhs.length);
        assert(rhsPos < rhs.length);

        // Copy seq, title and FASTQ QVs from subseq
        static_cast<FASTQSequence *>(this)->Copy(subseq);

        // Copy SMRT QVs
        if (rhs.preBaseFrames != NULL) {
            preBaseFrames = ProtectedNew<HalfWord>(length);
            memcpy(preBaseFrames, rhs.preBaseFrames, length * sizeof(HalfWord));
        }
        if (rhs.widthInFrames != NULL) {
            widthInFrames = ProtectedNew<HalfWord>(length);
            memcpy(widthInFrames, rhs.widthInFrames, length * sizeof(HalfWord));
        }
        if (rhs.pulseIndex != NULL) {
            pulseIndex = ProtectedNew<int>(length);
            memcpy(pulseIndex, rhs.pulseIndex, sizeof(int) * length);
        }
    }

    // Copy other member variables from rhs
    subreadStart_ = rhs.subreadStart_;
    subreadEnd_ = rhs.subreadEnd_;
    lowQualityPrefix = rhs.lowQualityPrefix;
    lowQualitySuffix = rhs.lowQualitySuffix;
    highQualityRegionScore = rhs.highQualityRegionScore;
    zmwData = rhs.zmwData;

    assert(deleteOnExit);  // should have control over seq and all QVs

    subseq.Free();
    copiedFromBam = rhs.copiedFromBam;
#ifdef USE_PBBAM
    bamRecord = rhs.bamRecord;
#endif
}

void SMRTSequence::Print(std::ostream &out) const
{
    out << "SMRTSequence for zmw " << HoleNumber() << ", [" << SubreadStart() << ", "
        << SubreadEnd() << ")" << std::endl;
    DNASequence::Print(out);
}

SMRTSequence &SMRTSequence::operator=(const SMRTSequence &rhs)
{
    SMRTSequence::Copy(rhs);
    return *this;
}

void SMRTSequence::Free()
{
    if (deleteOnExit == true) {
        if (preBaseFrames) {
            delete[] preBaseFrames;
        }
        if (widthInFrames) {
            delete[] widthInFrames;
        }
        if (pulseIndex) {
            delete[] pulseIndex;
        }
        if (startFrame) {
            delete[] startFrame;
        }
        // FIXME: memory of QVs should be handled within class
        //        in a consistent way.
        // Comments from Mark Chaisson:
        // meanSignal, maxSignal, midSignal and classifierQV
        // need to be handled separatedly.
    }

    // Reset SMRT QV pointers anyway
    preBaseFrames = NULL;
    widthInFrames = NULL;
    pulseIndex = NULL;
    startFrame = NULL;

    // Reset member variables
    subreadStart_ = subreadEnd_ = 0;
    lowQualityPrefix = lowQualitySuffix = 0;
    readScore = 0;
    highQualityRegionScore = 0;
    readGroupId_ = "";
    copiedFromBam = false;
#ifdef USE_PBBAM
    bamRecord = PacBio::BAM::BamRecord();
#endif

    // ZMWMetrics
    for (size_t i = 0; i < 4; i++) {
        hqRegionSnr_[i] = -1;
    }

    // Free seq, title and FASTQ QVs, also reset deleteOnExit.
    // Don't call FASTQSequence::Free() before freeing SMRT QVs.
    FASTQSequence::Free();
}

SMRTSequence &SMRTSequence::HoleNumber(UInt holeNumber)
{
    zmwData.holeNumber = holeNumber;
    return *this;
}

UInt SMRTSequence::HoleNumber(void) const { return zmwData.holeNumber; }

SMRTSequence &SMRTSequence::HoleXY(const int x, const int y)
{
    zmwData.x = x;
    zmwData.y = y;
    return *this;
}

UInt SMRTSequence::HoleX(void) const { return zmwData.x; }

UInt SMRTSequence::HoleY(void) const { return zmwData.y; }

SMRTSequence &SMRTSequence::HoleStatus(const unsigned char holeStatus)
{
    zmwData.holeStatus = holeStatus;
    return *this;
}

unsigned char SMRTSequence::HoleStatus(void) const { return zmwData.holeStatus; }

std::string SMRTSequence::MovieName(void) const { return SMRTTitle(GetTitle()).MovieName(); }

DNALength SMRTSequence::SubreadStart(void) const { return subreadStart_; }

SMRTSequence &SMRTSequence::SubreadStart(const DNALength start)
{
    subreadStart_ = start;
    return *this;
}

DNALength SMRTSequence::SubreadEnd(void) const { return subreadEnd_; }

SMRTSequence &SMRTSequence::SubreadEnd(const DNALength end)
{
    subreadEnd_ = end;
    return *this;
}

DNALength SMRTSequence::SubreadLength(void) const { return subreadEnd_ - subreadStart_; }

std::string SMRTSequence::ReadGroupId() const { return readGroupId_; }

SMRTSequence &SMRTSequence::ReadGroupId(const std::string &rid)
{
    readGroupId_ = rid;
    return *this;
}

float SMRTSequence::HQRegionSnr(const char base) const
{
    if (::toupper(base) == 'A')
        return hqRegionSnr_[SMRTSequence::SnrIndex4Base::A];
    else if (::toupper(base) == 'C')
        return hqRegionSnr_[SMRTSequence::SnrIndex4Base::C];
    else if (::toupper(base) == 'G')
        return hqRegionSnr_[SMRTSequence::SnrIndex4Base::G];
    else if (::toupper(base) == 'T')
        return hqRegionSnr_[SMRTSequence::SnrIndex4Base::T];
    else
        assert("Base must be in A, C, G, T" == 0);
}

SMRTSequence &SMRTSequence::HQRegionSnr(const char base, float v)
{
    if (::toupper(base) == 'A')
        hqRegionSnr_[SMRTSequence::SnrIndex4Base::A] = v;
    else if (::toupper(base) == 'C')
        hqRegionSnr_[SMRTSequence::SnrIndex4Base::C] = v;
    else if (::toupper(base) == 'G')
        hqRegionSnr_[SMRTSequence::SnrIndex4Base::G] = v;
    else if (::toupper(base) == 'T')
        hqRegionSnr_[SMRTSequence::SnrIndex4Base::T] = v;
    else
        assert("Base must be in A, C, G, T" == 0);
    return *this;
}

void SMRTSequence::MadeFromSubreadsAsPolymerase(const std::vector<SMRTSequence> &subreads)
{
    assert(subreads.size() > 0);
    DNALength hqStart = static_cast<DNALength>(-1), hqEnd = 0;
    bool hasInsDel = true, hasSubstitution = true;
    // Compute hqStart, hqEnd and which QVs to use over all subreads.
    for (auto subread : subreads) {
        hqStart = std::min(DNALength(subread.SubreadStart()), hqStart);
        hqEnd = std::max(DNALength(subread.SubreadEnd()), hqEnd);
        if (subread.insertionQV.Empty() || subread.deletionQV.Empty() ||
            subread.deletionTag == nullptr) {
            hasInsDel = false;
        }
        if (subread.substitutionTag == nullptr || subread.substitutionQV.Empty()) {
            hasSubstitution = false;
        }
    }
    this->Free();
    // Compact allocate memory.
    this->CompactAllocate(hqEnd, hasInsDel, hasSubstitution);
    memset(seq, 'N', sizeof(char) * hqEnd);
    this->lowQualityPrefix = hqStart;
    this->lowQualitySuffix = this->length - hqEnd;
    this->highQualityRegionScore = subreads[0].highQualityRegionScore;
    this->HoleNumber(subreads[0].HoleNumber());
    // Make title.
    std::stringstream ss;
    ss << SMRTTitle(subreads[0].GetTitle()).MovieName() << '/' << subreads[0].HoleNumber();
    this->CopyTitle(ss.str());

    // Copy subreads content to this polymerase read.
    for (auto subread : subreads) {
        memcpy(&this->seq[subread.SubreadStart()], &subread.seq[0], sizeof(char) * subread.length);
        if (hasInsDel) {
            this->insertionQV.Fill(subread.SubreadStart(), subread.length, subread.insertionQV, 0);
            this->deletionQV.Fill(subread.SubreadStart(), subread.length, subread.deletionQV, 0);
            memcpy(&this->deletionTag[subread.SubreadStart()], &subread.deletionTag[0],
                   sizeof(char) * subread.length);
        }
        if (hasSubstitution) {
            this->substitutionQV.Fill(subread.SubreadStart(), subread.length,
                                      subread.substitutionQV, 0);
            memcpy(&this->substitutionTag[subread.SubreadStart()], &subread.substitutionTag[0],
                   sizeof(char) * subread.length);
        }
    }
}

#ifdef USE_PBBAM
bool SMRTSequence::IsValid(const PacBio::BAM::BamRecord &record)
{
    DNALength expectedLength = 0;
    if (record.Type() != PacBio::BAM::RecordType::CCS) {
        expectedLength =
            static_cast<int>(record.QueryEnd()) - static_cast<int>(record.QueryStart());
    } else {
        expectedLength = static_cast<int>(record.Sequence().length());
    }
    if (record.Sequence().size() != expectedLength) return false;

    if (record.HasInsertionQV()) {
        if (record.InsertionQV().size() != expectedLength) return false;
    }
    if (record.HasDeletionQV()) {
        if (record.DeletionQV().size() != expectedLength) return false;
    }
    if (record.HasSubstitutionQV()) {
        if (record.SubstitutionQV().size() != expectedLength) return false;
    }
    if (record.HasMergeQV()) {
        if (record.MergeQV().size() != expectedLength) return false;
    }
    if (record.HasSubstitutionTag()) {
        if (record.SubstitutionTag().size() != expectedLength) return false;
    }
    if (record.HasDeletionTag()) {
        if (record.DeletionTag().size() != expectedLength) return false;
    }
    if (record.HasPreBaseFrames()) {
        if (record.PreBaseFrames().DataRaw().size() != expectedLength) return false;
    }
    if (record.HasPulseWidth()) {
        if (record.PulseWidth().DataRaw().size() != expectedLength) return false;
    }
    return true;
}

void SMRTSequence::Copy(const PacBio::BAM::BamRecord &record, bool copyAllQVs)
{
    Free();

    copiedFromBam = true;

    bamRecord = PacBio::BAM::BamRecord(record);

    // Only copy insertionQV, deletionQV, substitutionQV, mergeQV,
    // deletionTag and substitutionTag from BamRecord to SMRTSequence.
    // Do NOT copy other SMRTQVs such as startFrame, meanSignal...
    (static_cast<FASTQSequence *>(this))->Copy(record);

    // Set subread start, subread end in coordinate of zmw.
    if (record.Type() != PacBio::BAM::RecordType::CCS) {
        subreadStart_ = static_cast<int>(record.QueryStart());
        subreadEnd_ = static_cast<int>(record.QueryEnd());
    } else {
        subreadStart_ = 0;
        subreadEnd_ = static_cast<int>(record.Sequence().length());
    }

    // Shall we copy all pulse QVs including ipd and pw?
    if (copyAllQVs) {
        if (record.HasPreBaseFrames()) {
            std::vector<uint16_t> qvs = record.PreBaseFrames().DataRaw();
            assert(preBaseFrames == nullptr);
            preBaseFrames = ProtectedNew<HalfWord>(qvs.size());
            std::memcpy(preBaseFrames, &qvs[0], qvs.size() * sizeof(HalfWord));
        }
        if (record.HasPulseWidth()) {
            std::vector<uint16_t> qvs = record.PulseWidth().DataRaw();
            assert(widthInFrames == nullptr);
            widthInFrames = ProtectedNew<HalfWord>(qvs.size());
            std::memcpy(widthInFrames, &qvs[0], qvs.size() * sizeof(HalfWord));
        }
    }

    // preBaseQVs are not included in BamRecord, and will not be copied.
    // Copy read group id from BamRecord.
    ReadGroupId(record.ReadGroupId());

    // PacBio bam for secondary analysis does NOT carry zmw
    // info other than holeNumber, including holeStatus, holeX,
    // holeY, numEvents.
    UInt hn = static_cast<UInt>(record.HoleNumber());
    this->HoleNumber(hn)
        .
        // Assumption: holeStatus of a bam record must be 'SEQUENCING'
        HoleStatus(
            static_cast<unsigned char>(PacBio::AttributeValues::ZMW::HoleStatus::sequencingzmw))
        .
        // x = upper 16 bit, y = lower 16 bit
        HoleXY(hn >> 16, hn & 0x0000FFFF);

    // Set hq region read score
    if (record.HasReadAccuracy()) {
        // In pre 3.0.1 BAM, ReadAccuracy is in [0, 1000],
        // in post 3.0.1 BAM, ReadAccuracy is a float in [0, 1]
        // In blasr_libcpp, which supports both HDF5 and BAM,
        // readScore should always be a float in [0, 1],
        // and highQualityRegionScore always be a int in [0, 1000]
        readScore = float(record.ReadAccuracy());
        if (readScore <= 1.0) {
            highQualityRegionScore = int(readScore * 1000);
        } else {
            highQualityRegionScore = int(readScore);
            readScore /= 1000.0;
        }
    }

    // Set HQRegionSNR if record has the 'sn' tag
    if (record.HasSignalToNoise()) {
        // Signal to noise ratio of ACGT (in that particular ORDER) over
        // HQRegion from BAM: record.SignalToNoise()
        std::vector<float> snrs = record.SignalToNoise();
        this->HQRegionSnr('A', snrs[0])
            .HQRegionSnr('C', snrs[1])
            .HQRegionSnr('G', snrs[2])
            .HQRegionSnr('T', snrs[3]);
    }
}

#endif
