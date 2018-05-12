// Author: Mark Chaisson
// Modified by: Yuan Li

#include <pbdata/reads/RegionTable.hpp>

#include <algorithm>
#include <iostream>
#include <ostream>

RegionTable& RegionTable::Reset()
{
    map_.clear();
    columnNames.clear();
    regionTypes.clear();
    regionDescriptions.clear();
    regionSources.clear();
    regionTypeEnums.clear();
    return *this;
}

std::vector<RegionType> RegionTable::RegionTypeEnums(void) const { return regionTypeEnums; }

std::vector<std::string> RegionTable::RegionTypes(void) const { return regionTypes; }

std::vector<std::string> RegionTable::ColumnNames(void) const { return columnNames; }

std::vector<std::string> RegionTable::RegionDescriptions(void) const { return regionDescriptions; }

std::vector<std::string> RegionTable::RegionSources(void) const { return regionSources; }

RegionTable& RegionTable::ConstructTable(std::vector<RegionAnnotation>& table,
                                         const std::vector<std::string>& regionTypeStrs)
{
    RegionTypes(regionTypeStrs);  //< Set both regionTypes and regionTypeEnums.

    // Must sort region annotations by HoleNumber, RegionTypeIndex, Start, End, and Score
    std::sort(table.begin(), table.end(), compare_region_annotation_by_type);

    // Construct map_<holeNumber, RegionAnnotations>
    if (table.size() > 0) {
        UInt pre_hn = table[0].GetHoleNumber();
        auto itBegin = table.begin();
        for (auto it = table.begin(); it != table.end(); it++) {
            // Discrepency between data type of Regions/HoleNumber and ZMW/HoleNumber
            if (it->GetHoleNumber() > static_cast<int>(pre_hn)) {
                map_.insert(std::pair<UInt, RegionAnnotations>(
                    pre_hn, RegionAnnotations(pre_hn, std::vector<RegionAnnotation>(itBegin, it),
                                              regionTypeEnums)));
                pre_hn = it->GetHoleNumber();
                itBegin = it;
            }
        }

        map_.insert(std::pair<UInt, RegionAnnotations>(
            pre_hn, RegionAnnotations(pre_hn, std::vector<RegionAnnotation>(itBegin, table.end()),
                                      regionTypeEnums)));
    }
    return *this;
}

std::vector<RegionType> RegionTable::DefaultRegionTypes(void)
{
    std::vector<RegionType> ret;
    for (std::string regionTypeString : PacBio::AttributeValues::Regions::regiontypes) {
        ret.push_back(RegionTypeMap::ToRegionType(regionTypeString));
    }
    return ret;
}

RegionTable& RegionTable::RegionTypes(const std::vector<std::string>& regionTypeStrs)
{
    regionTypes = regionTypeStrs;
    for (std::string regionTypeString : regionTypeStrs) {
        regionTypeEnums.push_back(RegionTypeMap::ToRegionType(regionTypeString));
    }
    return *this;
}

RegionTable& RegionTable::ColumnNames(const std::vector<std::string>& in)
{
    columnNames = in;
    return *this;
}

RegionTable& RegionTable::RegionDescriptions(const std::vector<std::string>& in)
{
    regionDescriptions = in;
    return *this;
}

RegionTable& RegionTable::RegionSources(const std::vector<std::string>& in)
{
    regionSources = in;
    return *this;
}

bool RegionTable::HasHoleNumber(const UInt holeNumber) const
{
    return (map_.find(holeNumber) != map_.end());
}

RegionAnnotations RegionTable::operator[](const UInt holeNumber) const
{
    // Must check whether a zmw exists or not first.
    assert(HasHoleNumber(holeNumber) && "Could not find zmw in region table.");
    return map_.find(holeNumber)->second;
}
