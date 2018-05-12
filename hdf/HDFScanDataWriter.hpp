// Author: Yuan Li

#ifndef DATA_HDF_HDF_SCAN_DATA_WRITER_H_
#define DATA_HDF_HDF_SCAN_DATA_WRITER_H_

#include <iostream>
#include <string>

#include <pbdata/Enumerations.h>
#include <hdf/HDFAtom.hpp>
#include <hdf/HDFFile.hpp>
#include <hdf/HDFGroup.hpp>
#include <pbdata/reads/AcqParams.hpp>
#include <pbdata/reads/ScanData.hpp>

class HDFScanDataWriter
{
private:
    HDFGroup* rootGroupPtr;
    HDFGroup scanDataGroup;
    HDFGroup acqParamsGroup;
    HDFGroup dyeSetGroup;
    HDFGroup runInfoGroup;

    HDFAtom<std::string> whenStartedAtom;
    HDFAtom<float> frameRateAtom;
    HDFAtom<unsigned int> numFramesAtom;

    HDFAtom<std::string> baseMapAtom;
    HDFAtom<uint16_t> numAnalogAtom;

    HDFAtom<std::string> movieNameAtom;
    HDFAtom<std::string> runCodeAtom;

    HDFAtom<std::string> bindingKitAtom;
    HDFAtom<std::string> sequencingKitAtom;

    HDFAtom<unsigned int> platformIdAtom;
    HDFAtom<std::string> platformNameAtom;
    HDFAtom<std::string> instrumentNameAtom;

    void CreateAcqParamsGroup();

    void CreateDyeSetGroup();

    void CreateRunInfoGroup();

public:
    HDFScanDataWriter(HDFFile& _outFile);

    HDFScanDataWriter(HDFGroup& _rootGroup);

    ~HDFScanDataWriter();

    int Initialize(HDFGroup& _rootGroup);

    void Write(const ScanData& scanData);

    void Write(const ScanData& scanData, const AcqParams& acqParam);

    void WriteFrameRate(const float frameRate);

    void WriteNumFrames(const unsigned int numFrames);

    void WriteWhenStarted(const std::string whenStarted);

    void Close();

private:
    void WriteBaseMap(const std::string baseMapStr);

    void WriteNumAnalog(const uint16_t numAnalog);

    void WritePlatformId(const PlatformId id);

    void WriteMovieName(const std::string movieName);

    void WriteRunCode(const std::string runCode);

    void WriteBindingKit(const std::string& bindingKit);

    void WriteSequencingKit(const std::string& sequencingKit);

private:
    /// Write attributes to /ScanData/AcqParams
    void _WriteAcqParams(const AcqParams& acqParams);

    void _WriteAduGain(const float aduGain);

    void _WriteCameraGain(const float cameraGain);

    void _WriteCameraType(const int cameraType);

    void _WriteHotStartFrame(const UInt hotStartFrame);

    void _WriteLaserOnFrame(const UInt laserOnFrame);
};

#endif
