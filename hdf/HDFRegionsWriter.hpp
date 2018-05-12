// Author: Yuan Li

#ifndef _HDF_REGIONS_WRITER_HPP_
#define _HDF_REGIONS_WRITER_HPP_

#include <string>

#include <pbdata/Enumerations.h>
#include <hdf/HDF2DArray.hpp>
#include <hdf/HDFArray.hpp>
#include <hdf/HDFAtom.hpp>
#include <hdf/HDFFile.hpp>
#include <hdf/HDFWriterBase.hpp>
#include <pbdata/reads/RegionTable.hpp>

using namespace H5;

class HDFRegionsWriter : public HDFWriterBase
{
public:
    /// \name Constructor and destructor
    /// \{
    /// \param[in] filename, hdf file name
    /// \param[in] parentGroup, parent hdf group in hirarchy
    HDFRegionsWriter(const std::string &filename, HDFGroup &parentGroup,
                     const std::vector<std::string> &regionTypes =
                         PacBio::AttributeValues::Regions::regiontypes);
    ~HDFRegionsWriter(void);
    /// \}

private:
    /// \name Private variables for hdf IO.
    /// \{
    HDFGroup &parentGroup_;  //< parent hdf group

    /// A vector of std::string's of region types for RegionTypeIndex to look up. Order matters!
    std::vector<std::string> regionTypes_;

    HDF2DArray<int> regionsArray_;  //< HDF2DArray for writing regions to hdf

    int curRow_;  //< which row to write

    static const int NCOLS = 5;  //< number of columns in Regions table.

    /// \brief Write attributes of the 'regions' group
    bool WriteAttributes(void);
    /// \}

public:
    /// \name Method to write region annotations.
    /// \{
    /// \brief Append a vector of region annotations to 'regions'
    /// \param[in] annotations - region annotations to append.
    /// \returns true if succeeded.
    bool Write(const std::vector<RegionAnnotation> &annotations);

    /// \brief Append a region annotation to 'regions'
    /// \param[in] annotation - region annotation to append
    /// \returns true if succeeded.
    bool Write(const RegionAnnotation &annotation);

    void Flush(void);

    void Close(void);
};

#endif
