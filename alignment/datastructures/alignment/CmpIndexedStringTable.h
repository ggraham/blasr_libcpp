#ifndef DATASTRUCTURES_ALIGNMENT_CMP_INDEXED_STRING_TABLE_H_
#define DATASTRUCTURES_ALIGNMENT_CMP_INDEXED_STRING_TABLE_H_

#include <map>
#include <string>
#include <vector>

class CmpIndexedStringTable
{
public:
    void resize(int size)
    {
        names.resize(size);
        ids.resize(size);
    }

    void StoreArrayIndexMap()
    {
        for (size_t i = 0; i < ids.size(); i++) {
            idToArrayIndex[ids[i]] = i;
        }
    }

    //
    // The terminology of Index here is confusing.
    // Actually 'Index' is equivalent to 'id'.
    // Each id represents index of an indexed string.
    // That's why an id is called an 'Index' in this function.
    // GetNameAtIndex returns name of an indexed string,
    // whose index is the given value
    //
    bool GetNameAtIndex(int index, std::string &name)
    {
        std::map<int, int>::iterator mapIt;
        mapIt = idToArrayIndex.find(index);
        if (mapIt != idToArrayIndex.end()) {
            name = names[mapIt->second];
            return true;
        } else {
            return false;
        }
    }

    //
    // Here 'Id' means indexes of indexed strings,
    // 'Index' means index of an 'Id' in ids
    //
    bool GetIndexOfId(int id, int &index)
    {
        std::map<int, int>::iterator mapIt;
        mapIt = idToArrayIndex.find(id);
        if (mapIt != idToArrayIndex.end()) {
            index = mapIt->second;
            return true;
        } else {
            return false;
        }
    }
    std::vector<int> ids;
    std::vector<std::string> names;
    std::map<int, int> idToArrayIndex;
};

#endif
