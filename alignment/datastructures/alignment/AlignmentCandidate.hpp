#ifndef _ALIGNMENT_ALIGNMENT_CANDIDATE_HPP_
#define _ALIGNMENT_ALIGNMENT_CANDIDATE_HPP_

#include <algorithm>
#include "Alignment.hpp"
// pbdata
#include "../../../pbdata/DNASequence.hpp"
#include "../../../pbdata/FASTQSequence.hpp"

template <typename T_TSequence = FASTASequence, typename T_QSequence = FASTASequence>
class AlignmentCandidate : public blasr::Alignment
{

private:
    void ReassignSequence(DNASequence &curSeq, bool curIsSubstring, DNASequence &newSeq)
    {
        //
        // If this sequence is in control of itself (it is not a substring
        // of anoter sequence), it should be freed here to avoid memory
        // leaks.
        //
        if (curIsSubstring == false) {
            curSeq.Free();
        }
        curSeq.seq = newSeq.seq;
        curSeq.length = newSeq.length;
    }

    void ReassignSequence(FASTQSequence &curSeq, bool curIsSubstring, FASTQSequence &newSeq)
    {
        //
        // Free the current sequence with the same rules as above.
        //
        if (curIsSubstring == false) {
            curSeq.Free();
        }
        curSeq.ReferenceSubstring(newSeq, 0, newSeq.length);
    }

    // TryReadingQVs is a helper function for ReadOptionalQVs. It checks if the
    // qvs that are supposed to be copied to a member of AlignmentCandidate are
    // in fact empty. If so, then don't substr or copy anything
    void TryReadingQVs(const std::string qvs, DNALength start, DNALength length,
                       std::string *memberQVs)
    {
        if (qvs.size() == 0) {
            memberQVs->clear();
        } else {
            *memberQVs = qvs.substr(start, length);
        }
    }

public:
    T_TSequence tAlignedSeq;  // usually a reference substring of the full target sequence
    T_QSequence qAlignedSeq;  // usually a referenced substring of the full query sequence
    std::string insertionQV, deletionQV, mergeQV, substitutionQV, deletionTag, substitutionTag;
    std::vector<std::string> optionalQVNames;
    // qAlignedSeqPos is pos of the first base of qAlignedSeq (i.e., qAlignedSeq[0])
    // in coordinate of the full query sequence. Note that qAlignedSeq[0]) is **NOT**
    // always aligned.
    // qPos is pos of the first *ALIGNED* base in coordinate of qAlignedSeq.
    DNALength tAlignedSeqPos, qAlignedSeqPos;
    DNALength tAlignedSeqLength, qAlignedSeqLength;
    float pvalVariance, weightVariance, pvalNStdDev, weightNStdDev;
    int numSignificantClusters;
    int readIndex;
    //
    // [q/t]IsSubstring refers to whether or not the text or query
    // sequences are pointers into longer sequences.  If they are,
    // then reassigning the target/query sequences just means
    // reassigning the seq and length values.  If not, they occupy their
    // own space, and should have the same lifetime as the alignment
    // candidate object.
    //

    bool tIsSubstring, qIsSubstring;
    std::string tTitle, qTitle;
    float clusterScore, clusterWeight;
    //
    // For indexing into sequence index databases, tIndex stores which
    // target this match is from.
    //
    int tIndex;

    AlignmentCandidate &operator=(const AlignmentCandidate &rhs)
    {
        tAlignedSeq = rhs.tAlignedSeq;
        qAlignedSeq = rhs.qAlignedSeq;
        tAlignedSeqPos = rhs.tAlignedSeqPos;
        qAlignedSeqPos = rhs.qAlignedSeqPos;
        tAlignedSeqLength = rhs.tAlignedSeqLength;
        qAlignedSeqLength = rhs.qAlignedSeqLength;
        readIndex = rhs.readIndex;
        tIndex = rhs.tIndex;
        mapQV = rhs.mapQV;
        clusterScore = rhs.clusterScore;
        clusterWeight = rhs.clusterWeight;
        *((Alignment *)this) = ((Alignment &)rhs);

        pvalVariance = rhs.pvalVariance;
        pvalNStdDev = rhs.pvalNStdDev;
        weightVariance = rhs.weightVariance;
        weightNStdDev = rhs.weightNStdDev;

        insertionQV = rhs.insertionQV;
        deletionQV = rhs.deletionQV;
        substitutionQV = rhs.substitutionQV;
        mergeQV = rhs.mergeQV;
        substitutionTag = rhs.substitutionTag;
        deletionTag = rhs.deletionTag;

        return *this;
    }

    AlignmentCandidate()
    {
        /*
         * The default configuration of an alignment candidate is to have
         * the t and q sequences be substrings.  This means that the
         * position of the substrings starts at 0 (no offset into a longer
         * string).
         */
        tIsSubstring = true;
        qIsSubstring = true;
        tTitle = "";
        qTitle = "";
        tAlignedSeqPos = 0;
        qAlignedSeqPos = 0;
        tAlignedSeqLength = 0;
        qAlignedSeqLength = 0;
        tIndex = 0;
        readIndex = 0;
        mapQV = 50;
        clusterScore = 0;
        clusterWeight = 0;
        numSignificantClusters = 0;
        pvalVariance = pvalNStdDev = 0;
        weightVariance = weightNStdDev = 0;
    }

    std::string ToString(const bool showSeq = false, const bool showBlocks = false)
    {
        std::stringstream ss;
        std::string sep = "\n";
        ss << "AlignmentCandidate object "
           << "(mapQV " << mapQV << ", clusterscore " << clusterScore << ")" << sep
           << "qTitle: " << qTitle << ", tTitle: " << tTitle << sep << "qName: " << qName
           << ", qStrand: " << qStrand << sep << "tName: " << tName << ", tStrand: " << tStrand
           << sep << "qPos: " << qPos << ", qAlignedPos: " << qAlignedSeqPos
           << ", qAlignedSeqLength: " << qAlignedSeqLength
           << ", qAlignedSeq.length: " << qAlignedSeq.length << ", qLength: " << qLength << sep
           << "tPos: " << tPos << ", tAlignedPos: " << tAlignedSeqPos
           << ", tAlignedSeqLength: " << tAlignedSeqLength
           << ", tAlignedSeq.length: " << tAlignedSeq.length << ", tLength: " << tLength << sep;
        if (showSeq) {
            ss << "qAlignedSeq: " << qAlignedSeq.ToString() << sep
               << "tAlignedSeq: " << tAlignedSeq.ToString() << sep;
        }
        if (showBlocks) {
            ss << "Blocks: " << BlocksToString() << sep << "Gaps: " << GapsToString() << sep;
        }
        return ss.str();
    }

    void Print(std::ostream &out = std::cout, const bool showSeq = false,
               const bool showBlocks = false)
    {
        out << ToString(showSeq, showBlocks);
    }

    AlignmentCandidate(const AlignmentCandidate &rhs) { *this = rhs; }

    DNALength GenomicTBegin() { return tAlignedSeqPos + tPos; }

    DNALength GenomicTEnd() { return tAlignedSeqPos + tPos + TEnd(); }

    void GetQIntervalOnForwardStrand(int &qStart, int &qEnd) { GetQInterval(qStart, qEnd, true); }

    void GetQInterval(int &qStart, int &qEnd, bool useForwardStrand = false)
    {
        qStart = qEnd = 0;
        if (blocks.size() == 0) {
            return;
        }
        qStart = blocks[0].qPos + qAlignedSeqPos;
        qEnd = QEnd() + qAlignedSeqPos;
        if (useForwardStrand and qStrand == 1) {
            int forQEnd, forQStart;
            forQStart = qLength - qEnd;
            forQEnd = qLength - qStart;
            qStart = forQStart;
            qEnd = forQEnd;
        }
    }

    DNALength QAlignEnd() { return QEnd() + qPos + qAlignedSeqPos; }

    DNALength QAlignStart() { return qPos + qAlignedSeqPos; }

    DNALength TAlignStart() { return tPos + tAlignedSeqPos; }

    // Synonyms for T/QStart
    DNALength GetQBasesToStart() { return qPos + qAlignedSeqPos; }
    DNALength GetTBasesToStart() { return tPos + tAlignedSeqPos; }

    // ReadOptionalQVs populates the optional QV attributes of
    // AlignmentCandidate with values read from a vector.
    void ReadOptionalQVs(const std::vector<std::string> &optionalQVs, DNALength start,
                         DNALength length)
    {
        TryReadingQVs(optionalQVs[0], start, length, &insertionQV);
        TryReadingQVs(optionalQVs[1], start, length, &deletionQV);
        TryReadingQVs(optionalQVs[2], start, length, &substitutionQV);
        TryReadingQVs(optionalQVs[3], start, length, &mergeQV);
        TryReadingQVs(optionalQVs[4], start, length, &substitutionTag);
        TryReadingQVs(optionalQVs[5], start, length, &deletionTag);
    }

    // CopyQVs fills a vector with optional QV attributes.
    void CopyQVs(std::vector<std::string> *optionalQVs)
    {
        optionalQVNames.clear();
        optionalQVs->clear();
        optionalQVs->push_back(insertionQV);
        optionalQVNames.push_back("InsertionQV");
        optionalQVs->push_back(deletionQV);
        optionalQVNames.push_back("DeletionQV");
        optionalQVs->push_back(substitutionQV);
        optionalQVNames.push_back("SubstitutionQV");
        optionalQVs->push_back(mergeQV);
        optionalQVNames.push_back("MergeQV");
        optionalQVs->push_back(substitutionTag);
        optionalQVNames.push_back("SubstitutionTag");
        optionalQVs->push_back(deletionTag);
        optionalQVNames.push_back("DeletionTag");
    }

    void AppendAlignment(AlignmentCandidate &next)
    {

        //
        // If the next alignment is empty, just return now.
        //
        if (next.blocks.size() == 0) {
            return;
        }
        //
        // It is necessary to determine how much after the first alignment
        // the second alignment starts, in order to
        assert(GetTBasesToStart() <= next.GetTBasesToStart());
        assert(GetQBasesToStart() <= next.GetQBasesToStart());
        assert(GetTBasesToStart() + TEnd() <= next.GetTBasesToStart());
        assert(GetQBasesToStart() + QEnd() <= next.GetQBasesToStart());

        //
        // qOffset is the offset of the frame of reference for the 'next'
        // alignment relative to this one.  If the frame of reference is
        // the same (qOffset == 0 and tOffset == 0), then the alignment
        // blocks may simply be appended.  If it is non zero, then the
        // location in the query or target where the alignment has started
        // is different, and the values of qPos or tPos need to be
        // adjusted accordingly.

        DNALength qOffset = next.qPos + next.qAlignedSeqPos - qPos - qAlignedSeqPos;
        DNALength tOffset = next.tPos + next.tAlignedSeqPos - tPos - tAlignedSeqPos;
        DNALength origQEnd = QEnd();
        DNALength origTEnd = TEnd();

        /*    DNALength tGap = next.GetTBasesToStart() + next.blocks[0].tPos - (GetTBasesToStart() + TEnd());
              DNALength qGap = next.GetQBasesToStart() + next.blocks[0].qPos - (GetQBasesToStart() + QEnd());
              */
        DNALength tGap = next.GetTBasesToStart() - (GetTBasesToStart() + TEnd());
        DNALength qGap = next.GetQBasesToStart() - (GetQBasesToStart() + QEnd());

        //

        if (gaps.size() > 0 and next.gaps.size() > 0) {

            //
            // Determine the gap between the two alignments in order to assign
            // the gap between them.
            DNALength commonGap = 0;
            DNALength gapDiff = 0;

            if (tGap >= qGap) {
                gapDiff = tGap - qGap;
                commonGap = tGap - gapDiff;
                tGap = gapDiff;
                qGap = 0;
            } else {
                gapDiff = qGap - tGap;
                commonGap = qGap - gapDiff;
                qGap = gapDiff;
                tGap = 0;
            }

            if (commonGap > 0) {
                // There is some portion of sequence between the two alignments
                // that is a common gap. Add a block representing this.

                blasr::Block block;
                block.qPos = origQEnd;
                block.tPos = origTEnd;
                block.length = commonGap;
                blocks.push_back(block);
                blasr::GapList emptyGap;
                gaps.push_back(emptyGap);
            }

            blasr::GapList endGapList;
            //
            // When gapDiff == 0, an empty list is appended.
            //
            if (next.gaps.size() > 0) {
                //
                // The first gap in the next alignment is already handled by
                // the gap created with the endGap.  So get rid of the first
                // gaps.
                //        next.gaps[0].resize(0);
                AppendAlignmentGaps(next);
            }
        }

        //
        // Finally append all the blocks.
        //
        AppendAlignmentBlocks(next, qOffset, tOffset);
    }

    void FreeSubsequences()
    {
        if (tIsSubstring == false) {
            tAlignedSeq.Free();
        }
        if (qIsSubstring == false) {
            qAlignedSeq.Free();
        }
    }

    void ReassignTSequence(DNASequence &newSeq)
    {
        ReassignSequence(tAlignedSeq, tIsSubstring, newSeq);
    }

    template <typename T_Sequence>
    void ReassignQSequence(T_Sequence &newSeq)
    {
        ReassignSequence(qAlignedSeq, qIsSubstring, newSeq);
    }

    // This function forces query strand to be Forward.
    //
    // Note that historically, query sequence in Alignment is always forward
    // (i.e., qStrand == Forward), while target strand can be Reverse.
    // This has been changed to place gaps consistently (SAT-59),
    // query in Alignment may also be Reverse, while target strand is Forward.
    //
    // When query is Reverse, this function will reverse complement both query
    // and target sequences such that qStrand is Forward, and tStrand is Reverse.
    // Also need to recompute all query and target positions in coordinate of
    // rc query and rc target, recompute blocks, gaps, and reverse quality values.
    void MakeQueryForward(void)
    {
        if (qStrand == Forward) return;

        // Assert not both query and target are in Reverse strands.
        assert(qStrand == Reverse and tStrand == Forward);

        // Reverse query and target strands, make query strand forward.
        qStrand = Forward;
        tStrand = Reverse;

        // tAlignedSeqLength and qAlignedSeqLength should remain the same
        // tLength and qLength should remain the same
        //
        // Recompute tPos and qPos in coordinate of rc sequences.
        // Note that qPos and tPos in a block is relative to the beginning
        // of the alignment rather than the target or query.
        //
        // Below is an example.
        //            012345 678901234567
        // rc Query   NXXAAA-AAACCXXXNNNN
        //               |||x|||x|
        // fw Target     AAATAAAGC
        //               012345678
        //
        // * the full query sequence is 'NXXAAAAAACCXXXNNNN', qLength = 18
        // * qAligneSeqPos = 1, qAlignedSeq.length = 13, qAlignedSeq is 'XXAAAAAACCXXX',
        // * qPos = 2, the first aligned base is 'A',
        // * blocks: [(0, 0, 3), (3, 4, 3), (7, 8, 1)]
        //
        // After reversing both query and target sequences,
        //            012345678901 234567
        // fw Query   NNNNXXXGGTTT-TTTXXN
        //                   |x|||x|||
        // rc Target         GCTTTATTT
        //                   012345678
        // * Length of full rc query is unchanged, qLength = 18
        // * qAlignedSeqPos = 4, qAlignedSeq.length = 13 (unchanged), qAlignedSeq is 'XXXGGTTTTTTXX',
        // * qPos = 3, the first aligned base is 'G',
        // * blocks: [(0, 0, 1), (2, 2, 3), (5, 6, 3)]
        const DNALength _qPos = qAlignedSeq.length - (qPos + blocks[blocks.size() - 1].QEnd());
        const DNALength _tPos = tAlignedSeq.length - (tPos + blocks[blocks.size() - 1].TEnd());

        // Recompute tAlignedSeqPos, qAlignedSeqPos in coordinate of rc full query.
        const DNALength _qAlignedSeqPos = qLength - (qAlignedSeqPos + qAlignedSeq.length);
        const DNALength _tAlignedSeqPos = tLength - (tAlignedSeqPos + tAlignedSeq.length);

        std::vector<blasr::Block> _blocks;
        for (const blasr::Block &b : blocks) {
            // b.QEnd is an exclusive end point, get rc pos of the inclusive end point
            DNALength _rcQPos = qAlignedSeq.MakeRCCoordinate(qPos + b.QEnd() - 1) - _qPos;
            DNALength _rcTPos = tAlignedSeq.MakeRCCoordinate(tPos + b.TEnd() - 1) - _tPos;
            _blocks.push_back(blasr::Block(_rcQPos, _rcTPos, b.length));
        }
        std::reverse(_blocks.begin(), _blocks.end());

        // reverse GapList.
        std::reverse(gaps.begin(), gaps.end());

        // Reverse quality values.
        std::reverse(insertionQV.begin(), insertionQV.end());
        std::reverse(deletionQV.begin(), deletionQV.end());
        std::reverse(mergeQV.begin(), mergeQV.end());
        std::reverse(substitutionQV.begin(), substitutionQV.end());
        std::reverse(deletionTag.begin(), deletionTag.end());
        std::reverse(substitutionTag.begin(), substitutionTag.end());

        // Assign new values
        blocks.clear();
        std::move(_blocks.begin(), _blocks.end(), std::back_inserter(blocks));
        _blocks.clear();

        qPos = _qPos;
        tPos = _tPos;
        qAlignedSeqPos = _qAlignedSeqPos;
        tAlignedSeqPos = _tAlignedSeqPos;

        // Replace the original qAlignedSeq and tAlignedSeq with rc sequences.
        // Make reverse complementary sequences of qAlignedSeq and tAlignedSeq
        qAlignedSeq.ReverseComplementSelf();
        tAlignedSeq.ReverseComplementSelf();

        FreeSubsequences();
        tIsSubstring = qIsSubstring = false;

        // Remove end gaps in the end
        RemoveEndGaps();
    }

    ~AlignmentCandidate()
    {
        qAlignedSeq.Free();
        tAlignedSeq.Free();
    }
};
//
// Define a default alignment candidate for aligning quality-sequence
// to a reference without quality.
//
typedef AlignmentCandidate<DNASequence, FASTQSequence> T_AlignmentCandidate;

class SortAlignmentPointersByScore
{
public:
    /// Sort AlignmentCandidate pointer by score, then by target position.
    int operator()(T_AlignmentCandidate *lhs, T_AlignmentCandidate *rhs);
};

class SortAlignmentPointersByMapQV
{
public:
    /// Sort AlignmentCandidate pointer by mapQV, then by target position.
    int operator()(T_AlignmentCandidate *lhs, T_AlignmentCandidate *rhs);
};

#endif  // _ALIGNMENT_ALIGNMENT_CANDIDATE_HPP_
