// Author: Yuan Li

#include <pbdata/PrettyException.hpp>
#include <pbdata/reads/RegionTypeMap.hpp>

std::string RegionTypeMap::ToString(RegionType rt)
{
    assert(RegionTypeToString.find(rt) != RegionTypeToString.end());
    return RegionTypeToString.find(rt)->second;
}

RegionType RegionTypeMap::ToRegionType(const std::string& str)
{
    if (StringToRegionType.find(str) == StringToRegionType.end()) {
        std::cout << "Unsupported RegionType " << str << std::endl;
        assert(false);
    }
    return StringToRegionType.find(str)->second;
}

int RegionTypeMap::ToIndex(const std::string& typeStr, const std::vector<std::string>& typeStrs)
{
    auto it = std::find(typeStrs.begin(), typeStrs.end(), typeStr);
    if (it == typeStrs.end()) {
        BLASR_THROW("Could not find RegionType " + typeStr);
    } else {
        return std::distance(typeStrs.begin(), it);
    }
}

int RegionTypeMap::ToIndex(RegionType rt, const std::vector<std::string>& typeStrs)
{
    return RegionTypeMap::ToIndex(RegionTypeMap::ToString(rt), typeStrs);
}

int RegionTypeMap::ToIndex(RegionType rt, const std::vector<RegionType>& regionTypes)
{
    auto it = std::find(regionTypes.begin(), regionTypes.end(), rt);
    if (it == regionTypes.end()) {
        BLASR_THROW("Could not find RegionType " + RegionTypeMap::ToString(rt));
    } else {
        return std::distance(regionTypes.begin(), it);
    }
}

const std::map<RegionType, std::string> RegionTypeMap::RegionTypeToString = {
    {Adapter, "Adapter"}, {Insert, "Insert"}, {HQRegion, "HQRegion"}, {BarCode, "Barcode"}};

const std::map<std::string, RegionType> RegionTypeMap::StringToRegionType = {
    {"Adapter", Adapter}, {"Insert", Insert}, {"HQRegion", HQRegion}, {"Barcode", BarCode},
};
