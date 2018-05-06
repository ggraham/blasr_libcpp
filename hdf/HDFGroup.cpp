#include "HDFGroup.hpp"

using namespace H5;

HDFGroup::HDFGroup() : HDFAttributable() { groupIsInitialized = false; }

void HDFGroup::AddGroup(std::string groupName)
{
    group.createGroup(groupName);
    return;
}

H5Object *HDFGroup::GetObject() { return &group; }

int HDFGroup::Initialize(Group &fg, std::string groupName)
{
    try {
        group = fg.openGroup(groupName.c_str());
        groupIsInitialized = true;
        return 1;
    } catch (FileIException &e) {
        return 0;
    } catch (GroupIException &e) {
        return 0;
    } catch (Exception &e) {
        return 0;
    }
    return 1;
}

int HDFGroup::Initialize(HDFGroup &parentGroup, std::string groupName)
{
    return Initialize(parentGroup.group, groupName);
}

bool HDFGroup::ContainsObject(std::string queryObjectName)
{
    hsize_t objIdx;
    hsize_t numGroupObjs = group.getNumObjs();
    for (objIdx = 0; objIdx < numGroupObjs; objIdx++) {
        H5std_string groupObjectName;
        groupObjectName = group.getObjnameByIdx(objIdx);
        if (groupObjectName == queryObjectName) {
            return true;
        }
    }
    return false;
}

void HDFGroup::Close()
{
    if (groupIsInitialized) {
        group.close();
    }
}
