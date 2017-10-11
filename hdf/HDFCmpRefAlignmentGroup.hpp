#ifndef _BLASR_HDF_CMP_REF_ALIGNMENT_GROUP_HPP_
#define _BLASR_HDF_CMP_REF_ALIGNMENT_GROUP_HPP_

#include <map>
#include <string>

#include "H5Cpp.h"
#include "HDFCmpExperimentGroup.hpp"
#include "HDFData.hpp"
#include "HDFGroup.hpp"

class HDFCmpRefAlignmentGroup
{
public:
    HDFGroup refGroup;
    std::string refGroupName;
    std::vector<HDFCmpExperimentGroup*> readGroups;
    HDFAtom<std::string> annotationStringAtom;
    std::map<std::string, int> experimentNameToIndex;
    // A RefAlignmentGroup may contain one or more
    // ExperimentGroups. The following shows a
    // RefAlignmentGroup containing two ExperimentGroups.
    // /ref00001/m121219_103658_42194_c000447032559900001500000112311426_s1_p0
    // /ref00001/m121219_103658_42194_c000447032559900001500000112311426_s2_p0
    //
    // But these ExperimentGroups are not necessarily
    // grouped by movie. For example, the following groups
    // can be seen in deep-sorted cmp.h5
    // /ref00001/rg8953-0
    // /ref00001/rg2453-1

    int Initialize(H5::CommonFG& group, std::string _refGroupName)
    {
        refGroupName = _refGroupName;
        refGroup.Initialize(group, _refGroupName);
        //		annotationStringAtom.Initialize(refGroup.group, "annotationString");
        return 1;
    }

    void Create(HDFGroup parent, std::string refGroupNameP)
    {
        refGroupName = refGroupNameP;
        parent.AddGroup(refGroupName);
        refGroup.Initialize(parent, refGroupName);
    }

    HDFCmpExperimentGroup* GetExperimentGroup(std::string readGroupName)
    {
        //
        // In contrast to initialization, only create one group.
        //
        std::map<std::string, int>::iterator it = experimentNameToIndex.find(readGroupName);
        if (it != experimentNameToIndex.end()) {
            assert(it->second < int(readGroups.size()));
            return readGroups[it->second];
        }

        //
        // Allocate the new group structure
        //
        int newReadGroupIndex = readGroups.size();
        HDFCmpExperimentGroup* readGroupPtr = new HDFCmpExperimentGroup;
        if (readGroupPtr == nullptr) {
            std::cout << "ERROR, failed to allocate memory for HDFCmpExperimentGroup!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        readGroups.push_back(readGroupPtr);
        experimentNameToIndex[readGroupName] = newReadGroupIndex;

        //
        // Now add it to the cmp.h5 file.
        //
        if (readGroupPtr->Create(refGroup, readGroupName) == 0) {
            delete readGroupPtr;
            experimentNameToIndex[readGroupName] = -1;
            return NULL;
        }
        return readGroupPtr;
    }

    bool ContainsExperimentGroup(std::string readGroupName)
    {
        return experimentNameToIndex.find(readGroupName) != experimentNameToIndex.end();
    }

    HDFCmpExperimentGroup* InitializeExperimentGroup(std::string experimentGroupName,
                                                     std::set<std::string>& includedFields)
    {
        if (refGroup.ContainsObject(experimentGroupName)) {
            HDFCmpExperimentGroup* newGroup = new HDFCmpExperimentGroup;
            if (newGroup == nullptr) {
                std::cout << "ERROR, failed to allocate memory for HDFCmpExperimentGroup!" << std::endl;
                std::exit(EXIT_FAILURE);
            }
            if (newGroup->Initialize(refGroup, experimentGroupName, includedFields) == 0) {
                std::cout << "ERROR, could not initialize the exp group." << std::endl;
                std::exit(EXIT_FAILURE);
            }
            experimentNameToIndex[experimentGroupName] = readGroups.size();
            readGroups.push_back(newGroup);
            return newGroup;
        } else {
            return NULL;
        }
    }

    HDFCmpExperimentGroup* InitializeExperimentGroup(std::string experimentGroupName)
    {
        std::set<std::string> EMPTYIncludedFields;
        return InitializeExperimentGroup(experimentGroupName, EMPTYIncludedFields);
    }
};

#endif
