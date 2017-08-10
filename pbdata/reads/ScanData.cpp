// Author: Mark Chaisson

#include "ScanData.hpp"
#include <algorithm>
#include <iostream>

std::string ScanData::BaseMapToStr(const std::map<char, size_t>& baseMap)
{
    std::string baseMapStr = "    ";  //4 dye channels.
    for (auto it = baseMap.begin(); it != baseMap.end(); ++it) {
        if (it->second < 4) {
            baseMapStr[it->second] = it->first;
        }
    }
    std::string tmpBaseMap = baseMapStr;
    std::sort(tmpBaseMap.begin(), tmpBaseMap.end());
    std::transform(tmpBaseMap.begin(), tmpBaseMap.end(), tmpBaseMap.begin(), ::toupper);
    if (tmpBaseMap != "ACGT") {
        std::cout << "ERROR, invalid ScanData BaseMap " << baseMapStr << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return baseMapStr;
}

std::map<char, size_t> ScanData::StrToBaseMap(const std::string& baseMapStr)
{
    std::map<char, size_t> ret;
    for (size_t i = 0; i < baseMapStr.size(); i++) {
        ret[baseMapStr[i]] = i;
    }
    return ret;
}

bool ScanData::IsValidBaseMap(const std::map<char, size_t>& baseMap)
{
    const char X = 'x';
    std::string v(4, X);

    for (const char base : {'A', 'T', 'G', 'C'}) {
        size_t index = baseMap.find(base)->second;
        if (not(baseMap.find(base) != baseMap.end() and index <= 3))
            return false;
        else
            v[index] = 'o';
    }
    if (v.find(X) != std::string::npos)
        return false;
    else
        return true;
}

ScanData::ScanData(const AcqParams& acqParams) : acqParams_(acqParams)
{
    platformId = NoPlatform;
    frameRate = 0.0;
    numFrames = 0;
    movieName = runCode = whenStarted = "";
    baseMap.clear();
}

ScanData& ScanData::SetAcqParams(const AcqParams& acqParams)
{
    acqParams_ = acqParams;
    return *this;
}

AcqParams ScanData::GetAcqParams(void) const { return acqParams_; }

std::string ScanData::GetMovieName() { return movieName; }

ScanData& ScanData::PlatformID(const PlatformId& id)
{
    platformId = id;
    return *this;
}
ScanData& ScanData::FrameRate(const float& rate)
{
    frameRate = rate;
    return *this;
}
ScanData& ScanData::NumFrames(const unsigned int& num)
{
    numFrames = num;
    return *this;
}
ScanData& ScanData::MovieName(const std::string& name)
{
    movieName = name;
    return *this;
}
ScanData& ScanData::RunCode(const std::string& code)
{
    runCode = code;
    return *this;
}
ScanData& ScanData::WhenStarted(const std::string& when)
{
    whenStarted = when;
    return *this;
}
ScanData& ScanData::BaseMap(const std::map<char, size_t>& bmp)
{
    baseMap.clear();
    baseMap.insert(bmp.begin(), bmp.end());
    return *this;
}
ScanData& ScanData::BaseMap(const std::string& baseMapStr)
{
    return this->BaseMap(ScanData::StrToBaseMap(baseMapStr));
}
ScanData& ScanData::SequencingKit(const std::string sequencingKit)
{
    sequencingKit_ = sequencingKit;
    return *this;
}
ScanData& ScanData::BindingKit(const std::string bindingKit)
{
    bindingKit_ = bindingKit;
    return *this;
}

PlatformId ScanData::PlatformID(void) const { return platformId; }
float ScanData::FrameRate(void) const { return frameRate; }
unsigned int ScanData::NumFrames(void) const { return numFrames; }
std::string ScanData::MovieName(void) const { return movieName; }
std::string ScanData::RunCode(void) const { return runCode; }
std::string ScanData::WhenStarted(void) const { return whenStarted; }
std::map<char, size_t> ScanData::BaseMap(void) const { return baseMap; }

std::string ScanData::BaseMapStr(void) const { return ScanData::BaseMapToStr(baseMap); }

std::string ScanData::SequencingKit(void) const { return sequencingKit_; }
std::string ScanData::BindingKit(void) const { return bindingKit_; }
