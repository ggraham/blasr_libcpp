#ifndef _BLASR_DATASET_COLLECTION_HPP_
#define _BLASR_DATASET_COLLECTION_HPP_

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "HDFData.hpp"
#include "HDFGroup.hpp"

class DatasetCollection
{
public:
    std::vector<std::string> fieldNames;
    std::map<std::string, bool> includedFields;
    std::map<std::string, bool> requiredFields;

    void MakeFieldRequired(std::string &fieldName);

    void MakeFieldOptional(std::string &fieldName);

    void InitializeAllFields(bool value);

    void InitializeFields(std::vector<std::string> &fieldList);

    void InitializeFields(std::vector<char *> &fieldList);

    int IncludeField(std::string fieldName);

    bool FieldIsIncluded(std::string fieldName);

    bool ContainsField(std::string fieldName);

    template <typename T_Dataset>
    bool InitializeDataset(HDFGroup &group, T_Dataset &dataset, std::string datasetName);
};

#include "DatasetCollectionImpl.hpp"

#endif
