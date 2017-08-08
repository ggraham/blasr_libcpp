// Author: Mark Chaisson

#include "PulseFile.hpp"

void PulseFile::CopySignal(HalfWord *signalData,  // either a vector or matrix
                           int signalNDims,
                           DSLength pulseStartPos,  // 0 if baseToPulseIndex maps to abs position
                           int *baseToPulseIndex, Nucleotide *readSeq, DNALength readLength,
                           HalfWord *readData)
{
    // if baseToPulseIndex maps bases to absolute pulse postions
    // pulseStartPos must be 0;
    // otherwise, pulseStartPos is pulseStartPositions[holeIndex]

    std::map<char, size_t> baseMap = GetBaseMap();
    if (signalNDims == 1) {
        for (DNALength i = 0; i < readLength; i++) {
            readData[i] = signalData[pulseStartPos + baseToPulseIndex[i]];
        }
    } else {
        for (DNALength i = 0; i < readLength; i++) {
            readData[i] = signalData[baseToPulseIndex[i] * 4 + baseMap[readSeq[i]]];
        }
    }
}

void PulseFile::CopyReadAt(uint32_t plsReadIndex, int *baseToPulseIndex, SMRTSequence &read)
{
    DSLength pulseStartPos = pulseStartPositions[plsReadIndex];
    bool OK = true;
    if (midSignal.size() > 0) {
        assert(midSignal.size() > pulseStartPos);
        OK = Realloc(read.midSignal, read.length);
        CopySignal(&midSignal[0], midSignalNDims, pulseStartPos, baseToPulseIndex, read.seq,
                   read.length, read.midSignal);
    }

    if (maxSignal.size() > 0) {
        assert(maxSignal.size() > pulseStartPos);
        OK = OK and Realloc(read.maxSignal, read.length);
        CopySignal(&maxSignal[0], maxSignalNDims, pulseStartPos, baseToPulseIndex, read.seq,
                   read.length, read.maxSignal);
    }

    if (meanSignal.size() > 0) {
        assert(meanSignal.size() > pulseStartPos);
        OK = OK and Realloc(read.meanSignal, read.length);
        CopySignal(&meanSignal[0], meanSignalNDims, pulseStartPos, baseToPulseIndex, read.seq,
                   read.length, read.meanSignal);
    }
    if (plsWidthInFrames.size() > 0) {
        OK = OK and Realloc(read.widthInFrames, read.length);
        StoreField(plsWidthInFrames, baseToPulseIndex, read.widthInFrames, read.length);
    }
    if (classifierQV.size() > 0) {
        OK = OK and Realloc(read.classifierQV, read.length);
        StoreField(classifierQV, baseToPulseIndex, read.classifierQV, read.length);
    }
    if (startFrame.size() > 0) {
        OK = OK and Realloc(read.startFrame, read.length);
        StoreField(startFrame, baseToPulseIndex, read.startFrame, read.length);
    }

    if (not OK) {
        std::cout << "ERROR, failed to CopyReadAt(" << plsReadIndex << ")" << std::endl;
        exit(EXIT_FAILURE);
    }
}
