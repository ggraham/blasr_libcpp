// Author: Yuan Li

#include <hdf/HDFZMWMetricsWriter.hpp>
#include <pbdata/reads/ScanData.hpp>

HDFZMWMetricsWriter::HDFZMWMetricsWriter(const std::string& filename, HDFGroup& parentGroup,
                                         const std::map<char, size_t>& baseMap)
    : HDFWriterBase(filename), parentGroup_(parentGroup), baseMap_(baseMap), curRow_(0)
{
    if (not parentGroup.groupIsInitialized)
        PARENT_GROUP_NOT_INITIALIZED_ERROR(PacBio::GroupNames::zmwmetrics);
    else {
        parentGroup_.AddGroup(PacBio::GroupNames::zmwmetrics);

        if (zmwMetricsGroup_.Initialize(parentGroup_, PacBio::GroupNames::zmwmetrics) == 0)
            FAILED_TO_CREATE_GROUP_ERROR(PacBio::GroupNames::zmwmetrics);

        InitializeChildHDFGroups();
    }

    // Sanity Check BaseMap
    assert(ScanData::IsValidBaseMap(baseMap));
}

HDFZMWMetricsWriter::~HDFZMWMetricsWriter()
{
    Flush();  // Must flush in case group is empty.
    assert(WriteAttributes());
    Close();
}

bool HDFZMWMetricsWriter::WriteOneZmw(const SMRTSequence& read)
{
    try {
        float snrs[4];
        for (char base : {'A', 'C', 'G', 'T'}) {
            snrs[baseMap_[base]] = read.HQRegionSnr(base);
        }
        hqRegionSNRArray_.WriteRow(snrs, SNRNCOLS);
        readScoreArray_.Write(&read.readScore, 1);
        productivityArray_.Write(&read.zmwData.holeStatus, 1);
    } catch (H5::Exception& e) {
        AddErrorMessage("Failed to write HQRegionSNR or ReadScore or Productivity.");
        return false;
    }
    ++curRow_;

    return true;
}

void HDFZMWMetricsWriter::Flush(void)
{
    hqRegionSNRArray_.Flush();
    readScoreArray_.Flush();
    productivityArray_.Flush();
}

void HDFZMWMetricsWriter::Close(void)
{
    hqRegionSNRArray_.Close();
    readScoreArray_.Close();
    productivityArray_.Close();

    zmwMetricsGroup_.Close();
}

bool HDFZMWMetricsWriter::InitializeChildHDFGroups(void)
{
    bool OK = true;

    if (hqRegionSNRArray_.Initialize(zmwMetricsGroup_, PacBio::GroupNames::hqregionsnr, SNRNCOLS) ==
        0) {
        FAILED_TO_CREATE_GROUP_ERROR(PacBio::GroupNames::hqregionsnr);
        OK = false;
    }

    if (readScoreArray_.Initialize(zmwMetricsGroup_, PacBio::GroupNames::readscore) == 0) {
        FAILED_TO_CREATE_GROUP_ERROR(PacBio::GroupNames::readscore);
        OK = false;
    }

    if (productivityArray_.Initialize(zmwMetricsGroup_, PacBio::GroupNames::productivity) == 0) {
        FAILED_TO_CREATE_GROUP_ERROR(PacBio::GroupNames::productivity);
        OK = false;
    }

    return OK;
}

bool HDFZMWMetricsWriter::WriteAttributes(void)
{
    if (curRow_ > 0) {
        bool OK = AddAttribute<std::string>(
                      hqRegionSNRArray_, PacBio::AttributeNames::Common::description,
                      PacBio::AttributeValues::ZMWMetrics::HQRegionSNR::description) and
                  AddAttribute<std::string>(
                      readScoreArray_, PacBio::AttributeNames::Common::description,
                      PacBio::AttributeValues::ZMWMetrics::ReadScore::description) and
                  AddAttribute<std::string>(
                      productivityArray_, PacBio::AttributeNames::Common::description,
                      PacBio::AttributeValues::ZMWMetrics::Productivity::description);
        return OK;
    } else {
        AddErrorMessage("Could not write attributes when ZMWMetrics group is empty.");
        return false;
    }
}
