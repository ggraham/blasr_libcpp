#include "HDFPulseDataFile.hpp"

DSLength HDFPulseDataFile::GetAllReadLengths(std::vector<DNALength> &readLengths)
{
    nReads = static_cast<UInt>(zmwReader.numEventArray.arrayLength);
    readLengths.resize(nReads);
    zmwReader.numEventArray.Read(0, nReads, &readLengths[0]);
    return readLengths.size();
}

void HDFPulseDataFile::CheckMemoryAllocation(long allocSize, long allocLimit, const char *fieldName)
{
    if (allocSize > allocLimit) {
        if (fieldName == NULL) {
            std::cout << "Allocating too large of memory" << std::endl;
        } else {
            std::cout << "Allocate size " << allocSize << " > allocate limit " << allocLimit
                      << std::endl;
            std::cout << "ERROR! Reading the dataset " << fieldName << " will use too much memory."
                      << std::endl;
            std::cout << "The pls/bas file is too large, exiting." << std::endl;
        }
        std::exit(EXIT_FAILURE);
    }
}

HDFPulseDataFile::HDFPulseDataFile()
{
    pulseDataGroupName = "PulseData";
    nReads = 0;
    useScanData = false;
    closeFileOnExit = false;
    maxAllocNElements = INT_MAX;
    preparedForRandomAccess = false;
    rootGroupPtr = NULL;
}

void HDFPulseDataFile::PrepareForRandomAccess()
{
    std::vector<DNALength> offset_;
    GetAllReadLengths(offset_);
    // type of read length of a single read : DNALength
    // type of total read length of all reads in plx.h5: DSLength
    eventOffset.assign(offset_.begin(), offset_.end());
    DSLength curOffset = 0;
    for (size_t i = 0; i < eventOffset.size(); i++) {
        DSLength curLength = eventOffset[i];
        eventOffset[i] = curOffset;
        curOffset = curOffset + curLength;
    }
    nReads = static_cast<UInt>(eventOffset.size());
    preparedForRandomAccess = true;
}

int HDFPulseDataFile::OpenHDFFile(std::string fileName, const H5::FileAccPropList &fileAccPropList)
{

    try {
        H5::FileAccPropList propList = fileAccPropList;
        H5::Exception::dontPrint();
        hdfBasFile.openFile(fileName.c_str(), H5F_ACC_RDONLY, propList);
    } catch (H5::Exception &e) {
        std::cout << "ERROR, could not open hdf file" << fileName << ", exiting." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    closeFileOnExit = true;
    return 1;
}

//
// All pulse data files contain the "PulseData" group name.
//
//
int HDFPulseDataFile::InitializePulseDataFile(std::string fileName,
                                              const H5::FileAccPropList &fileAccPropList)
{

    if (OpenHDFFile(fileName, fileAccPropList) == 0) return 0;
    return 1;
}

int HDFPulseDataFile::Initialize(std::string fileName, const H5::FileAccPropList &fileAccPropList)
{

    if (InitializePulseDataFile(fileName, fileAccPropList) == 0) {
        return 0;
    }
    //
    // The pulse group is contained directly below the root group.
    //
    if (rootGroup.Initialize(hdfBasFile, "/") == 0) {
        return 0;
    }
    rootGroupPtr = &rootGroup;
    return Initialize();
}

//
// Initialize inside another open group.
//
int HDFPulseDataFile::Initialize(HDFGroup *rootGroupP)
{
    rootGroupPtr = rootGroupP;
    return Initialize();
}

//
// Initialize all fields
//
int HDFPulseDataFile::Initialize()
{
    preparedForRandomAccess = false;
    if (InitializePulseGroup() == 0) return 0;
    if (rootGroupPtr->ContainsObject("ScanData")) {
        if (scanDataReader.Initialize(rootGroupPtr) == 0) {
            return 0;
        } else {
            useScanData = true;
        }
    }
    return 1;
}

int HDFPulseDataFile::InitializePulseGroup()
{
    if (pulseDataGroup.Initialize(rootGroupPtr->group, pulseDataGroupName) == 0) return 0;
    return 1;
}

size_t HDFPulseDataFile::GetAllHoleNumbers(std::vector<unsigned int> &holeNumbers)
{
    CheckMemoryAllocation(zmwReader.holeNumberArray.arrayLength, maxAllocNElements,
                          "HoleNumbers (base)");
    holeNumbers.resize(nReads);
    zmwReader.holeNumberArray.Read(0, nReads, &holeNumbers[0]);
    return holeNumbers.size();
}

void HDFPulseDataFile::Close()
{
    if (useScanData) {
        scanDataReader.Close();
    }

    pulseDataGroup.Close();
    if (rootGroupPtr == &rootGroup) {
        rootGroup.Close();
    }
    /*
       std::cout << "there are " <<  hdfBasFile.getObjCount(H5F_OBJ_FILE) << " open files upon closing." <<std::endl;
       std::cout << "there are " <<  hdfBasFile.getObjCount(H5F_OBJ_DATASET) << " open datasets upon closing." <<std::endl;
       std::cout << "there are " <<  hdfBasFile.getObjCount(H5F_OBJ_GROUP) << " open groups upon closing." <<std::endl;
       std::cout << "there are " <<  hdfBasFile.getObjCount(H5F_OBJ_DATATYPE) << " open datatypes upon closing." <<std::endl;
       std::cout << "there are " <<  hdfBasFile.getObjCount(H5F_OBJ_ATTR) << " open attributes upon closing." <<std::endl;
       */
    if (closeFileOnExit) {
        hdfBasFile.close();
    }
}
