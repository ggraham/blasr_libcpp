#ifndef _BLASR_HDF_CMP_ROOT_GROUP_HPP_
#define _BLASR_HDF_CMP_ROOT_GROUP_HPP_

#include "../alignment/datastructures/alignment/CmpFile.hpp"
#include "HDF2DArray.hpp"
#include "HDFAtom.hpp"
template <typename T_Alignment>
class HDFCmpRootGroup
{
public:
    HDFGroup rootGroup;
    HDFAtom<std::string> version;
    HDFAtom<std::string> index;
    HDFAtom<std::string> readType;
    HDFAtom<std::string> commandLine;
    HDF2DArray<std::string> fileLog;

    ~HDFCmpRootGroup() { rootGroup.Close(); }

    int Initialize(H5::H5File &cmpFile)
    {
        if (rootGroup.Initialize(cmpFile, "/") == 0) {
            return 0;
        }
        if (rootGroup.ContainsObject("Version")) {
            if (version.Initialize(rootGroup.group, "Version") == 0) {
                return 0;
            }
        }
        if (rootGroup.ContainsObject("Index")) {
            if (index.Initialize(rootGroup.group, "Index") == 0) {
                return 0;
            }
        }
        if (rootGroup.ContainsObject("ReadType")) {
            if (readType.Initialize(rootGroup.group, "ReadType") == 0) {
                return 0;
            }
        }
        if (rootGroup.ContainsObject("CommandLine")) {
            if (commandLine.Initialize(rootGroup.group, "CommandLine") == 0) {
                return 0;
            }
        }

        //
        // For now, disable file log initialization until
        // hdf2darray<std::string> is tested more thoroughly
        //
        // if (fileLog.Initialize(rootGroup.group, "FileLog") == 0) {
        // return 0;}
        //
        return 1;
    }

    void ReadAttributes(CmpFile &cmpFile)
    {
        if (rootGroup.ContainsObject("Version")) {
            version.Read(cmpFile.version);
        }
        if (rootGroup.ContainsObject("Index")) {
            index.Read(cmpFile.index);
        }
        if (rootGroup.ContainsObject("ReadType")) {
            std::string readTypeString;
            readType.Read(readTypeString);
            cmpFile.StoreReadType(readTypeString);
        }
        if (rootGroup.ContainsObject("CommandLine")) {
            commandLine.Read(cmpFile.commandLine);
        }
    }
};

#endif
