#include <hdf/HDFData.hpp>

using namespace H5;

H5Object *HDFData::GetObject() { return &dataset; }

HDFData::HDFData(Group *_container, const std::string &_datasetName)
{
    container = _container;
    datasetName = _datasetName;
    fileDataSpaceInitialized = false;
    isInitialized = false;
}

HDFData::HDFData()
{
    container = NULL;
    fileDataSpaceInitialized = false;
    isInitialized = false;
}

bool HDFData::IsInitialized() const { return isInitialized; }

//
// Allow derived classes to be initialized generically.
//
int HDFData::Initialize(HDFGroup &parentGroup, const std::string &datasetName)
{
    (void)(parentGroup);
    (void)(datasetName);
    std::cout << "ERROR! Only a subclass should call this." << std::endl;
    std::exit(EXIT_FAILURE);
}

int HDFData::BaseInitializeDataset(Group &hdfFile, const std::string &_datasetName)
{
    dataset = hdfFile.openDataSet(_datasetName.c_str());
    isInitialized = true;
    fileDataSpaceInitialized = true;
    return 1;
}

int HDFData::InitializeDataset(HDFGroup &group, const std::string &_datasetName)
{
    return InitializeDataset(group.group, _datasetName);
}

int HDFData::InitializeDataset(Group &hdfFile, const std::string &_datasetName)
{
    try {
        datasetName = _datasetName;
        dataset = hdfFile.openDataSet(_datasetName.c_str());
        isInitialized = true;
        fileDataSpaceInitialized = true;
    } catch (FileIException &e) {
        std::cerr << e.getDetailMsg() << std::endl;
        return 0;
    } catch (GroupIException &e) {
        std::cerr << e.getDetailMsg() << std::endl;
        return 0;
    } catch (H5::Exception &e) {
        std::cerr << e.getDetailMsg() << std::endl;
        return 0;
    }
    return 1;
}

void HDFData::Close()
{
    if (isInitialized) {
        dataspace.close();
        dataset.close();
        isInitialized = false;
    }
}
