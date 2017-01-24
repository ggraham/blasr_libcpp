#ifndef _BLASR_CMP_FILE_HPP_
#define _BLASR_CMP_FILE_HPP_

#include "CmpIndexedStringTable.h"
// pbdata
#include "../../../pbdata/Enumerations.h"
#include "../../../pbdata/reads/ReadType.hpp"
#include "../../../pbdata/saf/AlnGroup.hpp"
#include "../../../pbdata/saf/AlnInfo.hpp"
#include "../../../pbdata/saf/MovieInfo.hpp"
#include "../../../pbdata/saf/RefGroup.hpp"
#include "../../../pbdata/saf/RefInfo.hpp"

#include <vector>

class CmpFile
{
public:
    int lastRow;
    std::string readTypeString, index, version, commandLine;
    ReadType::ReadTypeEnum readType;

    void StoreReadType(std::string &readTypeStringP);

    CmpIndexedStringTable readGroupTable, movieNameTable, refSeqTable;
    vector<string> colNames;
    PlatformId platformId;
    AlnGroup alnGroup;
    AlnInfo alnInfo;
    RefGroup refGroup;
    RefInfo refInfo;
    MovieInfo movieInfo;
};

#endif
