#ifndef _BLASR_CMP_SUPPORTED_FIELDS_HPP_
#define _BLASR_CMP_SUPPORTED_FIELDS_HPP_

#include <set>
#include <string>

class HDFCmpSupportedFields : public std::set<std::string>
{
public:
    HDFCmpSupportedFields();
};

#endif
