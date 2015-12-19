// Author: Yuan Li

#include "HDFWriterBase.hpp"


HDFWriterBase::~HDFWriterBase(void) 
{ }

std::vector<std::string> HDFWriterBase::Errors(void) const {
    return errors_;
}

bool HDFWriterBase::AddChildGroup(HDFGroup & parentGroup, 
                                  HDFGroup & childGroup,
                                  const std::string & childGroupName) {
    parentGroup.AddGroup(childGroupName);
    if (childGroup.Initialize(parentGroup, childGroupName) == 0) {
        FAILED_TO_CREATE_GROUP_ERROR(childGroupName);
        return false;
    }
    return true;
}

template<>
bool HDFWriterBase::AddAttribute(HDFData & group, 
                                 const std::string & attributeName, 
                                 const std::vector<std::string> & attributeValues)
{
    try {
        HDFAtom<std::vector<std::string> > attributeAtom;
        attributeAtom.Create(group.dataset, std::string(attributeName), attributeValues);
        attributeAtom.Close();
    }
    catch (H5::Exception &e) {
        FAILED_TO_CREATE_ATTRIBUTE_ERROR(attributeName);
        return false;
    }
    return true;
}

template<>
bool HDFWriterBase::AddAttribute(HDFGroup & group, 
                                 const std::string & attributeName, 
                                 const std::vector<std::string> & attributeValues)
{
    try {
        HDFAtom<std::vector<std::string> > attributeAtom;
        attributeAtom.Create(group.group, std::string(attributeName), attributeValues);
        attributeAtom.Close();
    }
    catch (H5::Exception &e) {
        FAILED_TO_CREATE_ATTRIBUTE_ERROR(attributeName);
        return false;
    }
    return true;
}

void HDFWriterBase::AddErrorMessage(const std::string & errmsg) {
    errors_.push_back(errmsg);
}

void HDFWriterBase::FAILED_TO_CREATE_GROUP_ERROR(const std::string & groupName) {
    std::stringstream ss;
    ss << "Failed to create group " << groupName << " in " << filename_;
    AddErrorMessage(ss.str());
}

void HDFWriterBase::FAILED_TO_CREATE_ATTRIBUTE_ERROR(const std::string & attributeName) {
    std::stringstream ss;
    ss << "Failed to create attribute " << attributeName << " in " << filename_;
    AddErrorMessage(ss.str());
}

void HDFWriterBase::PARENT_GROUP_NOT_INITIALIZED_ERROR(const std::string & groupName) {
    std::stringstream ss;
    ss << "Parent hdf group of " << groupName << " in file " << filename_
       << " is not initialized.";
    AddErrorMessage(ss.str());
}
