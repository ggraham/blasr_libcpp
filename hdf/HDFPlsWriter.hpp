#ifndef _BLASR_HDF_PLS_WRITER_HPP_
#define _BLASR_HDF_PLS_WRITER_HPP_

#error \
    "Where is data/??? Apparently this file is no longer needed, since it probably cannot compile."

#include <sstream>

#include <hdf/BufferedHDF2DArray.h>
#include <hdf/BufferedHDFArray.h>
#include <hdf/HDF2DArray.h>
#include <hdf/HDFArray.h>
#include <hdf/HDFAtom.h>
#include <hdf/HDFFile.h>
#include <hdf/PlatformId.h>
#include <pbdata/FASTQSequence.h>
#include <pbdata/utils/SMRTReadUtils.h>

using namespace H5;

class HDFPlsWriter
{
    HDFFile outFile;
    std::string hdfFileName;
    std::string movieName, runCode;
    PlatformId platformId;
    static const int bufferSize = 16;

    BufferedHDFArray<int> nElemArray;
    BufferedHDFArray<int> zmwXCoordArray;
    BufferedHDFArray<int> zmwYCoordArray;
    BufferedHDFArray<char> baseArray;
    BufferedHDFArray<unsigned char> qualArray;

    /*
	HDFArray<int> nElemArray;
	HDFArray<int> zmwXCoordArray;
	HDFArray<int> zmwYCoordArray;
	HDFArray<char> baseArray;
	HDFArray<unsigned char> qualArray;
	*/
    HDFAtom<std::string> movieNameAtom, runCodeAtom;

    //
    // Astro specific arrays.
    //
    BufferedHDF2DArray<uint16_t> holeXY2D;

    //
    // Springfield specific arrays.
    //
    BufferedHDFArray<unsigned int> holeNumberArray;

    //
    // Define arrays for rich quality values.
    //

    BufferedHDFArray<unsigned char> deletionQVArray;
    BufferedHDFArray<unsigned char> deletionTagArray;
    BufferedHDFArray<unsigned char> insertionQVArray;
    BufferedHDFArray<unsigned char> substitutionTagArray;
    BufferedHDFArray<unsigned char> substitutionQVArray;

    BufferedHDF2DArray<unsigned char> preBaseDeletionQVArray;
    HDFGroup rootGroup;
    Group runInfoGroup;
    Group baseCallGroup;
    Group zmwGroup;

public:
    ~HDFPlsWriter()
    {
        nElemArray.Flush();
        zmwXCoordArray.Flush();
        zmwYCoordArray.Flush();
        baseArray.Flush();
        qualArray.Flush();
        deletionQVArray.Flush();
        deletionTagArray.Flush();
        insertionQVArray.Flush();
        substitutionTagArray.Flush();
        substitutionQVArray.Flush();
        holeNumberArray.Flush();
    }

    HDFPlsWriter()
    {
        /*
		 * Default to astro for now.  This may need to change to a NO_ID
		 * platform, in which case it must be set with Initialize().
		 */
        platformId = Astro;
    }
    void AddMovieName(std::string movieName)
    {
        movieNameAtom.Create(runInfoGroup, "MovieName", movieName);
    }
    /*
	 * Initialization without a runCode is implicitly a springfield
	 * platform.  You can change it if you really want.
	 */
    void Initialize(std::string _hdfFileName, std::string movieName,
                    PlatformId _platformId = SpringfieldPlatform)
    {
        Initialize(_hdfFileName, _platformId);
        AddMovieName(movieName);
    }
    void Initialize(std::string _hdfFileName, std::string movieName, std::string runCode,
                    PlatformId _platformId = Astro)
    {
        Initialize(_hdfFileName, _platformId);
        if (movieName != "" and runCode != "") AddRunInfo(movieName, runCode);
    }

    void AddRunInfo(std::string movieName, std::string runCode)
    {
        AddMovieName(movieName);
        runCodeAtom.Create(runInfoGroup, "RunCode", runCode);
    }

    void Initialize(std::string _hdfFileName, PlatformId _platformId)
    {
        hdfFileName = _hdfFileName;
        platformId = _platformId;
        outFile.Create(hdfFileName);
        rootGroup.Initialize(*outFile.hdfFile, "/");
        rootGroup.AddGruop("PulseData");
        rootGroup.AddGroup("PulseData/BaesCalls");
        rootGroup.AddGroup("PulseData/BaseCalls/ZMW");
        rootGroup.AddGroup("ScanData/RunInfo");
        outFile.OpenGroup("ScanData/RunInfo", runInfoGroup);
        outFile.OpenGroup("PulseData/BaseCalls", baseCallGroup);
        outFile.OpenGroup("PulseData/BaseCalls/ZMW", zmwGroup);

        nElemArray.Initialize(&zmwGroup, "NumEvent", bufferSize);
        baseArray.Initialize(&baseCallGroup, "Basecall", bufferSize);
        qualArray.Initialize(&baseCallGroup, "QualityValue", bufferSize);

        deletionQVArray.Initialize(&baseCallGroup, "DeletionQV", bufferSize);
        deletionTagArray.Initialize(&baseCallGroup, "DeletionTag", bufferSize);
        insertionQVArray.Initialize(&baseCallGroup, "InsertionQV", bufferSize);
        preBaseDeletionQVArray.Initialize(&baseCallGroup, "PreBaseDeletionQV", 4, bufferSize);
        substitutionTagArray.Initialize(&baseCallGroup, "SubstitutionTag", bufferSize);
        substitutionQVArray.Initialize(&baseCallGroup, "SubstitutionQV", bufferSize);

        if (platformId == Astro) {
            holeXY2D.Initialize(&zmwGroup, "HoleXY", 2, bufferSize);
        } else if (platformId == SpringfieldPlatform) {
            holeNumberArray.Initialize(&zmwGroup, "HoleNumber", bufferSize);
        }
    }

    int Write(FASTQSequence &seq)
    {
        int lenArray[1] = {seq.length};
        nElemArray.Write(lenArray, 1);
        qualArray.Write(seq.qual, seq.length);
        baseArray.Write((const char *)seq.seq, seq.length);

        if (seq.deletionQV != NULL) {
            deletionQVArray.Write(seq.deletionQV, seq.length);
        }
        if (seq.preBaseDeletionQV != NULL) {
            DNALength readPos;
            for (readPos = 0; readPos < seq.length; readPos++) {
                preBaseDeletionQVArray.WriteRow(&seq.preBaseDeletionQV[readPos * 4], 4);
            }
        }
        if (seq.deletionTag != NULL) {
            deletionTagArray.Write(seq.deletionTag, seq.length);
        }
        if (seq.insertionQV != NULL) {
            insertionQVArray.Write(seq.insertionQV, seq.length);
        }
        if (seq.substitutionQV != NULL) {
            substitutionQVArray.Write(seq.substitutionQV, seq.length);
        }
        if (seq.substitutionTag != NULL) {
            substitutionTagArray.Write(seq.substitutionTag, seq.length);
        }

        if (platformId == Astro) {
            // now extract the x an y coordinates.
            int x, y;
            GetSMRTReadCoordinates(seq, x, y);
            uint16_t xy[2] = {(uint16_t)x, (uint16_t)y};
            holeXY2D.WriteRow(xy, 2);
            int holeNumber = 0;
            seq.GetHoleNumber(holeNumber);
            holeNumberArray.Write(&holeNumber, 1);
        } else if (platformId == SpringfieldPlatform) {
            unsigned int holeNumber;
            GetSpringfieldHoleNumberFromTitle(seq, holeNumber);
            holeNumberArray.Write(&holeNumber, 1);
        }
        // For now say this always works. HDF will choke if a problem
        // happens.
        return 1;
    }
};

#endif
