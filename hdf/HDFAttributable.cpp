#include "HDFAttributable.hpp"
#include <cassert>

using namespace H5;

void CallStoreAttributeName(H5Location &obj, std::string attrName, void *attrList)
{
    (void)(obj);
    ((std::vector<std::string> *)attrList)->push_back(std::string(attrName));
}

void HDFAttributable::StoreAttributeNames(H5Location &thisobject,
                                          const std::vector<std::string> &attributeNames)
{
    int nAttr = thisobject.getNumAttrs();
    unsigned int bounds[2];
    bounds[0] = 0;
    bounds[1] = nAttr;
    attributeNameList.clear();
    thisobject.iterateAttrs(&CallStoreAttributeName, bounds, (void *)&attributeNames);
}

H5Location *HDFAttributable::GetObject() { return NULL; }

int HDFAttributable::ContainsAttribute(const std::string &attributeName)
{
    size_t i;
    std::vector<std::string> tmpAttributeNames;
    try {
        H5Location *obj = GetObject();
        assert(obj != NULL);
        StoreAttributeNames(*obj, tmpAttributeNames);
        for (i = 0; i < tmpAttributeNames.size(); i++) {
            if (tmpAttributeNames[i] == attributeName) return true;
        }
    } catch (H5::Exception e) {
        //Failed to read attribute // e.printError();
    }
    return false;
}
