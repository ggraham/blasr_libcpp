#include <hdf/HDFAttributable.hpp>

#include <cassert>

using namespace H5;

// This template is required in order to juggle between HDF5 1.8 and 1.10.
//
// HDF5 1.8 has for attr_operator_t
//
//   typedef void(* H5::attr_operator_t)(H5Location &loc, const H5std_string attr_name, void *operator_data)
//
// whereas HDF5 1.10 has for attr_operator_t
//
//   typedef void(* H5::attr_operator_t)(H5Object &loc, const H5std_string attr_name, void *operator_data)

template <typename T>
void CallStoreAttributeName(T &obj, std::string attrName, void *attrList)
{
    (void)(obj);
    ((std::vector<std::string> *)attrList)->push_back(std::string(attrName));
}

void HDFAttributable::StoreAttributeNames(H5Object &thisobject,
                                          const std::vector<std::string> &attributeNames)
{
    int nAttr = thisobject.getNumAttrs();
    unsigned int bounds[2];
    bounds[0] = 0;
    bounds[1] = nAttr;
    attributeNameList.clear();
    thisobject.iterateAttrs(&CallStoreAttributeName, bounds, (void *)&attributeNames);
}

H5Object *HDFAttributable::GetObject() { return NULL; }

int HDFAttributable::ContainsAttribute(const std::string &attributeName)
{
    size_t i;
    std::vector<std::string> tmpAttributeNames;
    try {
        H5Object *obj = GetObject();
        assert(obj != NULL);
        StoreAttributeNames(*obj, tmpAttributeNames);
        for (i = 0; i < tmpAttributeNames.size(); i++) {
            if (tmpAttributeNames[i] == attributeName) return true;
        }
    } catch (const H5::Exception &e) {
        //Failed to read attribute // e.printError();
    }
    return false;
}
