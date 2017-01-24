// Author: Yuan Li

#include "HDFRegionsWriter.hpp"

HDFRegionsWriter::HDFRegionsWriter(const std::string &filename, HDFGroup &parentGroup,
                                   const std::vector<std::string> &regionTypes)
    : HDFWriterBase(filename), parentGroup_(parentGroup), regionTypes_(regionTypes), curRow_(0)
{
    // Initialize the 'regions' group.
    regionsArray_.Initialize(parentGroup_, PacBio::GroupNames::regions, RegionAnnotation::NCOLS);
}

HDFRegionsWriter::~HDFRegionsWriter(void)
{
    WriteAttributes();
    Close();
}

bool HDFRegionsWriter::WriteAttributes(void)
{
    if (curRow_ > 0) {
        AddAttribute<std::vector<std::string>>(regionsArray_,
                                               PacBio::AttributeNames::Regions::columnnames,
                                               PacBio::AttributeValues::Regions::columnnames);
        AddAttribute<std::vector<std::string>>(
            regionsArray_, PacBio::AttributeNames::Regions::regiontypes, regionTypes_);
        AddAttribute<std::vector<std::string>>(
            regionsArray_, PacBio::AttributeNames::Regions::regiondescriptions,
            PacBio::AttributeValues::Regions::regiondescriptions);
        AddAttribute<std::vector<std::string>>(regionsArray_,
                                               PacBio::AttributeNames::Regions::regionsources,
                                               PacBio::AttributeValues::Regions::regionsources);
        return true;
    } else {
        AddErrorMessage("Could not write attributes when Regions group is empty.");
        return false;
    }
}

bool HDFRegionsWriter::Write(const std::vector<RegionAnnotation> &annotations)
{
    for (auto annotation : annotations)
        if (not Write(annotation)) return false;
    return true;
}

bool HDFRegionsWriter::Write(const RegionAnnotation &annotation)
{
    try {
        regionsArray_.WriteRow(annotation.row, HDFRegionsWriter::NCOLS);
    } catch (H5::Exception &e) {
        std::ostringstream sout;
        sout << "Failed to write region annotation " << annotation.GetHoleNumber();
        AddErrorMessage(sout.str());
        return false;
    }
    ++curRow_;
    return true;
}

void HDFRegionsWriter::Flush(void) { regionsArray_.Flush(); }

void HDFRegionsWriter::Close(void)
{
    Flush();
    regionsArray_.Close();
}
