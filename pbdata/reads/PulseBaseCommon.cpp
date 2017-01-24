// Author: Mark Chaisson

#include "PulseBaseCommon.hpp"
#include <algorithm>
#include <vector>

float PulseBaseCommon::GetFrameRate() { return scanData.frameRate; }

unsigned int PulseBaseCommon::GetNumFrames() { return scanData.numFrames; }

std::string PulseBaseCommon::GetMovieName() { return scanData.movieName; }

std::map<char, size_t> PulseBaseCommon::GetBaseMap() { return scanData.baseMap; }

bool PulseBaseCommon::LookupReadIndexByHoleNumber(uint32_t holeNumber, int &readIndex)
{
    std::vector<uint32_t>::iterator holeIt;
    if (holeNumbers.size() == 0) {
        return false;
    }
    holeIt = lower_bound(holeNumbers.begin(), holeNumbers.end(), holeNumber);
    if (holeIt == holeNumbers.end()) {
        return false;
    }
    if (*holeIt == holeNumber) {
        readIndex = holeIt - holeNumbers.begin();
        return true;
    } else {
        return false;
    }
}
