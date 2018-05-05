#ifndef _BLASR_HDF_GROUP_HPP_
#define _BLASR_HDF_GROUP_HPP_

#include <H5Cpp.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "../pbdata/StringUtils.hpp"
#include "HDFAttributable.hpp"

class HDFGroup : public HDFAttributable
{
public:
    std::vector<std::string> objectNames;
    std::string objectName;
    H5::Group group;
    bool groupIsInitialized;

    HDFGroup();

    void AddGroup(std::string groupName);

    H5::H5Object* GetObject();

    int Initialize(H5::Group& fg, std::string groupName);

    int Initialize(HDFGroup& parentGroup, std::string groupName);

    bool ContainsObject(std::string queryObjectName);

    void Close();
};

#endif
