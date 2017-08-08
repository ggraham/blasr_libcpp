#ifndef _BLASR_HDF_CMP_FILE_HPP_
#define _BLASR_HDF_CMP_FILE_HPP_

#include <cassert>
#include <iostream>
#include <map>
#include <sstream>

#include <H5Cpp.h>

#include "HDF2DArray.hpp"
#include "HDFAlnGroupGroup.hpp"
#include "HDFAlnInfoGroup.hpp"
#include "HDFArray.hpp"
#include "HDFAtom.hpp"
#include "HDFCmpData.hpp"
#include "HDFCmpExperimentGroup.hpp"
#include "HDFCmpRefAlignmentGroup.hpp"

#include "HDFCmpRootGroup.hpp"
#include "HDFCmpSupportedFields.hpp"
#include "HDFFileLogGroup.hpp"
#include "HDFMovieInfoGroup.hpp"
#include "HDFRefGroupGroup.hpp"
#include "HDFRefInfoGroup.hpp"

// pbdata/
#include "../pbdata/SMRTSequence.hpp"
#include "../pbdata/alignment/CmpAlignment.hpp"  // not ../alignment!
#include "../pbdata/saf/RefInfo.hpp"

// alignment/datastructures/alignment  -- Yes, seriously.
#include "../alignment/datastructures/alignment/Alignment.hpp"
#include "../alignment/datastructures/alignment/AlignmentCandidate.hpp"
#include "../alignment/datastructures/alignment/ByteAlignment.h"
#include "../alignment/datastructures/alignment/CmpFile.hpp"
#include "../alignment/datastructures/alignment/CmpReadGroupTable.h"
#include "../alignment/datastructures/alignment/CmpRefSeqTable.h"

using namespace H5;

template <typename T_Alignment>
class HDFCmpFile : public HDFCmpData
{
public:
    std::map<int, int> movieNameIdToArrayIndex, readGroupPathIdToArrayIndex, refGroupIdToArrayIndex;
    std::map<std::string, int> refNameToArrayIndex;
    //  std::map<std::string,std::string> readGroupPathToReadGroup;
    std::map<int, std::string> alnGroupIdToReadGroupName;

    HDFAlnGroupGroup alnGroupGroup;
    HDFAlnInfoGroup alnInfoGroup;
    HDFMovieInfoGroup movieInfoGroup;
    HDFRefGroupGroup refGroupGroup;
    HDFRefInfoGroup refInfoGroup;
    HDFCmpRootGroup<T_Alignment> rootGroup;
    std::set<std::string> includedFields;
    HDFCmpSupportedFields supportedFields;
    HDFAtom<std::string> readTypeAtom;
    HDFFileLogGroup fileLogGroup;

    void AstroInitializeColumnNameMap()
    {
        CmpAlignmentBase::columnNameToIndex["AlignmentId"] = 0;
        CmpAlignmentBase::columnNameToIndex["ReadGroupId"] = 1;
        CmpAlignmentBase::columnNameToIndex["MovieId"] = 2;
        CmpAlignmentBase::columnNameToIndex["RefSeqId"] = 3;
        CmpAlignmentBase::columnNameToIndex["tStart"] = 4;
        CmpAlignmentBase::columnNameToIndex["tEnd"] = 5;
        CmpAlignmentBase::columnNameToIndex["AlignedStrand"] = 6;
        CmpAlignmentBase::columnNameToIndex["ExpId"] = 7;
        CmpAlignmentBase::columnNameToIndex["RunId"] = 8;
        CmpAlignmentBase::columnNameToIndex["Panel"] = 9;
        CmpAlignmentBase::columnNameToIndex["x"] = 10;
        CmpAlignmentBase::columnNameToIndex["y"] = 11;
        CmpAlignmentBase::columnNameToIndex["SubreadId"] = 12;
        CmpAlignmentBase::columnNameToIndex["rStart"] = 13;
        CmpAlignmentBase::columnNameToIndex["rEnd"] = 14;
        CmpAlignmentBase::columnNameToIndex["Z"] = 15;
        CmpAlignmentBase::columnNameToIndex["nM"] = 16;
        CmpAlignmentBase::columnNameToIndex["nMM"] = 17;
        CmpAlignmentBase::columnNameToIndex["nIns"] = 18;
        CmpAlignmentBase::columnNameToIndex["nDel"] = 19;
        CmpAlignmentBase::columnNameToIndex["offset_begin"] = 20;
        CmpAlignmentBase::columnNameToIndex["offset_end"] = 21;
    }

    void SpringfieldInitializeColumnNameMap()
    {
        CmpAlignmentBase::columnNameToIndex["AlignmentId"] = 0;
        CmpAlignmentBase::columnNameToIndex["ReadGroupId"] = 1;
        CmpAlignmentBase::columnNameToIndex["AlnGroupID"] = 1;
        CmpAlignmentBase::columnNameToIndex["AlnGroupId"] = 1;
        CmpAlignmentBase::columnNameToIndex["MovieId"] = 2;
        CmpAlignmentBase::columnNameToIndex["RefSeqId"] = 3;
        CmpAlignmentBase::columnNameToIndex["tStart"] = 4;
        CmpAlignmentBase::columnNameToIndex["tEnd"] = 5;
        CmpAlignmentBase::columnNameToIndex["RCRefStrand"] = 6;
        CmpAlignmentBase::columnNameToIndex["HoleNumber"] = 7;
        CmpAlignmentBase::columnNameToIndex["SetNumber"] = 8;
        CmpAlignmentBase::columnNameToIndex["StrobeNumber"] = 9;
        CmpAlignmentBase::columnNameToIndex["SubreadId"] = 10;
        CmpAlignmentBase::columnNameToIndex["rStart"] = 11;
        CmpAlignmentBase::columnNameToIndex["rEnd"] = 12;
        CmpAlignmentBase::columnNameToIndex["MapQV"] = 13;
        CmpAlignmentBase::columnNameToIndex["nM"] = 14;
        CmpAlignmentBase::columnNameToIndex["nMM"] = 15;
        CmpAlignmentBase::columnNameToIndex["nIns"] = 16;
        CmpAlignmentBase::columnNameToIndex["nDel"] = 17;
        CmpAlignmentBase::columnNameToIndex["offset_begin"] = 18;
        CmpAlignmentBase::columnNameToIndex["offset_end"] = 19;
        CmpAlignmentBase::columnNameToIndex["nBackRead"] = 20;
        CmpAlignmentBase::columnNameToIndex["nReadOverlap"] = 21;

        // All synonyms, the code requiring these should be refactored so
        // that it no synonyms are needed.
        //
        CmpAlignmentBase::columnNameToIndex["AlnID"] = 0;
        CmpAlignmentBase::columnNameToIndex["AlnGroupID"] = 1;
        CmpAlignmentBase::columnNameToIndex["MovieID"] = 2;
        CmpAlignmentBase::columnNameToIndex["RefGroupID"] = 3;
        CmpAlignmentBase::columnNameToIndex["RefGroupId"] = 3;
        CmpAlignmentBase::columnNameToIndex["MoleculeID"] = 10;
        CmpAlignmentBase::columnNameToIndex["Offset_begin"] = 18;
        CmpAlignmentBase::columnNameToIndex["Offset_end"] = 19;
    }

    int Initialize(std::string &hdfCmpFileName, std::set<std::string> includedFieldsP,
                   unsigned int flags = H5F_ACC_RDONLY,
                   const H5::FileAccPropList &fileAccPropList = H5::FileAccPropList::DEFAULT)
    {
        Initialize(hdfCmpFileName, flags, fileAccPropList);
        includedFields = includedFieldsP;
        return 1;
    }

    void Create(std::string &hdfCmpFileName)
    {
        H5File newFile(hdfCmpFileName.c_str(), H5F_ACC_TRUNC, FileCreatPropList::DEFAULT,
                       FileAccPropList::DEFAULT);
        hdfCmpFile.openFile(hdfCmpFileName.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        rootGroup.Initialize(hdfCmpFile);
        refGroupGroup.Create(rootGroup.rootGroup);
        alnGroupGroup.Create(rootGroup.rootGroup);
        refInfoGroup.Create(rootGroup.rootGroup);
        movieInfoGroup.Create(rootGroup.rootGroup);
        alnInfoGroup.Create(rootGroup.rootGroup);
        fileLogGroup.Create(rootGroup.rootGroup);

        //
        // Add atom data
        //
        readTypeAtom.Create(rootGroup.rootGroup.group, "ReadType");
        readTypeAtom.Write("standard");

        // Add some proxy data for now.
        HDFAtom<std::string> primaryVersionAtom, versionAtom;
        primaryVersionAtom.Create(rootGroup.rootGroup.group, "PrimaryVersion");
        primaryVersionAtom.Write("2.0.0.135558");
        versionAtom.Create(rootGroup.rootGroup.group, "Version");
        versionAtom.Write("2.0.0");
    }

    void SetReadType(std::string readType) { readTypeAtom.Write(readType.c_str()); }

    void GenerateNextRefGroupName(std::string &name)
    {
        std::stringstream nameStrm;
        nameStrm << "ref";
        nameStrm.width(6);
        nameStrm.fill('0');
        nameStrm << std::right << refGroupIdToArrayIndex.size() + 1;
        name = nameStrm.str();
    }

    int AddReference(std::string refName, unsigned int length, std::string md5, std::string &refGroupName)
    {
        //
        // Adding a reference requires:
        //
        // 1. Creating a new refgroup name.
        // 2. Create the new reference group to add alignments to.
        // 3. Adding this new name to the set of paths of ref groups.
        // 4. Adding information for this ref group.
        //

        // 1.
        this->GenerateNextRefGroupName(refGroupName);

        // 2.
        HDFCmpRefAlignmentGroup *newGroup = new HDFCmpRefAlignmentGroup;
        if (newGroup == nullptr) {
            std::cout << "ERROR, unable to allocate memory for cmp.h5 file." << std::endl;
            exit(EXIT_FAILURE);
        }
        newGroup->Create(rootGroup.rootGroup, refGroupName);
        refAlignGroups.push_back(newGroup);
        unsigned int id = refAlignGroups.size();

        // 3.
        refGroupGroup.AddPath("/" + refGroupName, id);
        refGroupIdToArrayIndex[id] = id - 1;
        refNameToArrayIndex[refName] = id - 1;
        // 4.
        refInfoGroup.AddRefInfo(refName, id, length, md5);
        return id;
    }

    void StoreAlnArray(std::vector<unsigned char> &alnArray, std::string refName, std::string &experimentName,
                       unsigned int &offsetBegin, unsigned int &offsetEnd)
    {
        //
        // First find the reference group.
        //
        assert(refNameToArrayIndex.find(refName) != refNameToArrayIndex.end());
        size_t refIndex = refNameToArrayIndex[refName];
        assert(refIndex < refAlignGroups.size());
        HDFCmpExperimentGroup *expGroup;
        expGroup = refAlignGroups[refIndex]->GetExperimentGroup(experimentName);
        expGroup->AddAlignment(alnArray, offsetBegin, offsetEnd);
    }

    // Write a vector of quality values to the appropriate experiment group.
    // This is similar to StoreAlignment, but for QVs
    void StoreQVs(const std::vector<UChar> &qvArray, const std::string &refName, const std::string &fieldName,
                  const std::string &experimentName, unsigned int *offsetBegin, unsigned int *offsetEnd)
    {
        assert(refNameToArrayIndex.find(refName) != refNameToArrayIndex.end());
        size_t refIndex = refNameToArrayIndex[refName];
        assert(refIndex < refAlignGroups.size());
        HDFCmpExperimentGroup *expGroup;
        expGroup = refAlignGroups[refIndex]->GetExperimentGroup(experimentName);
        expGroup->AddQVs(qvArray, fieldName, offsetBegin, offsetEnd);
    }

    // Write a vector of tag to the appropriate experiment group.
    // This is similar to StoreAlignment, but for DeletionTag and
    // SubstitutionTag.
    void StoreTags(const std::vector<char> &qvArray, const std::string &refName, const std::string &fieldName,
                   const std::string &experimentName, unsigned int *offsetBegin, unsigned int *offsetEnd)
    {
        assert(refNameToArrayIndex.find(refName) != refNameToArrayIndex.end());
        size_t refIndex = refNameToArrayIndex[refName];
        assert(refIndex < refAlignGroups.size());
        HDFCmpExperimentGroup *expGroup;
        expGroup = refAlignGroups[refIndex]->GetExperimentGroup(experimentName);
        expGroup->AddTags(qvArray, fieldName, offsetBegin, offsetEnd);
    }

    int Initialize(std::string &hdfCmpFileName, unsigned int flags = H5F_ACC_RDONLY,
                   const H5::FileAccPropList fileAccPropList = H5::FileAccPropList::DEFAULT)
    {
        /*
         * Initialize access to the HDF file.
         */
        try {
            hdfCmpFile.openFile(hdfCmpFileName.c_str(), flags, fileAccPropList);
        } catch (Exception &e) {
            std::cout << e.getDetailMsg() << std::endl;
            return 0;
        }

        rootGroup.Initialize(hdfCmpFile);
        readTypeAtom.Initialize(rootGroup.rootGroup, "ReadType");

        if (alnGroupGroup.Initialize(rootGroup.rootGroup) == 0) {
            std::cout << "ERROR, could not read AlnGroup of the cmp file." << std::endl;
            exit(EXIT_FAILURE);
        }
        if (refInfoGroup.Initialize(rootGroup.rootGroup) == 0) {
            std::cout << "ERROR, could not read RefInfo of the cmp file." << std::endl;
            exit(EXIT_FAILURE);
        }
        if (refGroupGroup.Initialize(rootGroup.rootGroup) == 0) {
            std::cout << "ERROR, could not read RefGroup of the cmp file." << std::endl;
            exit(EXIT_FAILURE);
        }
        if (movieInfoGroup.Initialize(rootGroup.rootGroup) == 0) {
            std::cout << "ERROR, could not read MovieInfo of the cmp file." << std::endl;
            exit(EXIT_FAILURE);
        }
        if (alnInfoGroup.Initialize(rootGroup.rootGroup) == 0) {
            std::cout << "ERROR, could not read AlnInfo of the cmp file." << std::endl;
            exit(EXIT_FAILURE);
        }
        if (fileLogGroup.Initialize(rootGroup.rootGroup) == 0) {
            std::cout << "ERROR, could not read FileLog of the cmp file." << std::endl;
            exit(EXIT_FAILURE);
        }

        SpringfieldInitializeColumnNameMap();

        return 1;
    }

    bool HasNoAlignments()
    {
        // return true if there is no alignment in the cmp file.
        if (alnInfoGroup.alnIndexArray.GetNRows() == 0) {
            return true;
        }
        return false;
    }

    unsigned int GetAlignmentIndexSize()
    {
        if (alnInfoGroup.alnInfoGroup.groupIsInitialized == false) {
            std::cout << "ERROR, getting the size of an index before initializing the cmp.h5 file."
                 << std::endl;
            exit(EXIT_FAILURE);
        }
        return alnInfoGroup.alnIndexArray.GetNRows();
    }

    // Add synonym
    unsigned int GetNAlignments() { return GetAlignmentIndexSize(); }

    static void ParseReadGroupPath(std::string &path, std::string &refName, std::string &readGroupName)
    {
        size_t delimPos;
        delimPos = path.find_last_of('/');
        if (delimPos != path.npos) {
            refName = path.substr(0, delimPos);
            //
            // Check the ref name to see if it has a preceeding '/'
            //
            readGroupName = path.substr(delimPos + 1, path.size());
        } else {
            refName = "";
            readGroupName = "";
        }
    }

    void StorePlatformId(CmpFile &cmpFile)
    {
        //
        // For now, hard wire as a springfield.  Later if the fields
        // change per platform, this may be updated.
        //
        cmpFile.platformId = Springfield;
    }

    void ReadAlignmentDescriptions(CmpFile &cmpFile)
    {

        //
        // Gather run information.
        //
        rootGroup.ReadAttributes(cmpFile);

        std::string readTypeString;
        readTypeAtom.Read(readTypeString);
        cmpFile.StoreReadType(readTypeString);
        //
        // Read in the groups that describe what alignments are present.
        //

        alnGroupGroup.Read(cmpFile.alnGroup);
        alnInfoGroup.Read(cmpFile.alnInfo);
        refGroupGroup.Read(cmpFile.refGroup);
        movieInfoGroup.Read(cmpFile.movieInfo);
        refInfoGroup.Read(cmpFile.refInfo);
        StorePlatformId(cmpFile);
    }

    void ReadStructure(CmpFile &cmpFile)
    {
        //
        // Now for every reference group in the cmp file, create a group.
        //
        std::map<std::string, int> refNameToArrayIndex;

        unsigned int refSeqIndex;

        for (refSeqIndex = 0; refSeqIndex < cmpFile.refGroup.path.size(); refSeqIndex++) {
            HDFCmpRefAlignmentGroup *refAlignGroup;
            refAlignGroup = new HDFCmpRefAlignmentGroup;
            if (refAlignGroup == nullptr) {
                std::cout << "ERROR, unable to allocate memory for cmp.h5 file." << std::endl;
                exit(EXIT_FAILURE);
            }
            refAlignGroup->Initialize(rootGroup.rootGroup.group,
                                      cmpFile.refGroup.path[refSeqIndex]);
            int refAlignGroupIndex = refAlignGroups.size();
            refAlignGroups.push_back(refAlignGroup);
            //
            // Allow one to index directly into this group given a string
            // representing the reference name.
            //
            refNameToArrayIndex[cmpFile.refGroup.path[refSeqIndex]] = refAlignGroupIndex;
            refGroupIdToArrayIndex[cmpFile.refGroup.id[refSeqIndex]] = refAlignGroupIndex;
        }

        //
        // Now that all ref groups are created, add read groups that
        // are aligned to the ref groups.
        //
        // Note that groups are not necessarily named by movie, especially in
        // a deep-sorted cmp.h5, read groups may look like:
        // /ref00001/rg8953-0
        // /ref00001/rg2453-1
        //
        unsigned int alnGroupIndex;
        for (alnGroupIndex = 0; alnGroupIndex < cmpFile.alnGroup.path.size(); alnGroupIndex++) {
            std::string refName, readGroupName;
            ParseReadGroupPath(cmpFile.alnGroup.path[alnGroupIndex], refName, readGroupName);

            //
            // Create an index that maps alnGroupId (/AlnGroup/Id) to readGroupName
            //
            alnGroupIdToReadGroupName[cmpFile.alnGroup.id[alnGroupIndex]] = readGroupName;

            //
            // Look up the group where this alignment should be found.
            //
            unsigned int refGroupArrayIndex;
            if (refNameToArrayIndex.find(refName) != refNameToArrayIndex.end()) {
                refGroupArrayIndex = refNameToArrayIndex[refName];
            } else {
                std::cout << "ERROR! The reference name '" << refName
                     << "' does not have an entry though it is "
                     << " specified in the path for " << cmpFile.readGroupTable.names[alnGroupIndex]
                     << std::endl;
                assert(0);
            }

            refAlignGroups[refGroupArrayIndex]->InitializeExperimentGroup(readGroupName,
                                                                          includedFields);
            // experimentNameToIndex should have been set in InitializeExperimentGroup();
            assert(static_cast<size_t>(
                       refAlignGroups[refGroupArrayIndex]->experimentNameToIndex[readGroupName]) ==
                   refAlignGroups[refGroupArrayIndex]->readGroups.size() - 1);
            refAlignGroups[refGroupArrayIndex]->experimentNameToIndex[readGroupName] =
                refAlignGroups[refGroupArrayIndex]->readGroups.size() - 1;
        }
    }

    void Read(CmpFile &cmpFile, bool readAllAlignments = true)
    {

        ReadAlignmentDescriptions(cmpFile);
        ReadStructure(cmpFile);

        if (!readAllAlignments) return;

        /*
         * Now that the alignment indices are all read in, read the base-by-base alignments.
         */

        unsigned int alignmentIndex;

        for (alignmentIndex = 0; alignmentIndex < cmpFile.alnInfo.alignments.size();
             alignmentIndex++) {
            unsigned int alnGroupId = cmpFile.alnInfo.alignments[alignmentIndex].GetAlnGroupId();
            unsigned int refGroupId = cmpFile.alnInfo.alignments[alignmentIndex].GetRefGroupId();
            std::string refSeqName;

            //
            // Make sure the refGroupId specified in the alignment index exists in the alignment groups.
            //
            int refGroupArrayIndex;
            if (refGroupIdToArrayIndex.find(refGroupId) == refGroupIdToArrayIndex.end()) {
                std::cout << "ERROR! Alignment "
                     << cmpFile.alnInfo.alignments[alignmentIndex].GetAlignmentId()
                     << " has ref seq id " << refGroupId << " that does not exist in the HDF file."
                     << std::endl;
                assert(0);
            } else {
                refGroupArrayIndex = refGroupIdToArrayIndex[refGroupId];
            }

            //
            // Point to the refGroup that this alignment is part of.
            //
            HDFCmpRefAlignmentGroup *refAlignGroup = refAlignGroups[refGroupArrayIndex];

            //
            // Now locate the read group that is part of this ref align group.
            //
            std::string readGroupName = alnGroupIdToReadGroupName[alnGroupId];

            if (refAlignGroup->experimentNameToIndex.find(readGroupName) ==
                refAlignGroup->experimentNameToIndex.end()) {
                std::cout << "Internal ERROR! The read group name " << readGroupName
                     << " is specified as part of "
                     << " the path in alignment "
                     << cmpFile.alnInfo.alignments[alignmentIndex].GetAlignmentId()
                     << " though it does not exist in the ref align group specified for this "
                        "alignment."
                     << std::endl;
                assert(0);
            }

            int experimentIndex;
            experimentIndex = refAlignGroup->experimentNameToIndex[readGroupName];

            unsigned int alignmentLength =
                cmpFile.alnInfo.alignments[alignmentIndex].GetOffsetEnd() -
                cmpFile.alnInfo.alignments[alignmentIndex].GetOffsetBegin();

            std::vector<unsigned char> alignmentArray;
            std::vector<UChar> fieldArray;

            if (alignmentArray.size() < alignmentLength) {
                alignmentArray.resize(alignmentLength);
            }

            /* 
             * Read in the base by base alignment.
             */

            refAlignGroup->readGroups[experimentIndex]->alignmentArray.Read(
                cmpFile.alnInfo.alignments[alignmentIndex].GetOffsetBegin(),
                cmpFile.alnInfo.alignments[alignmentIndex].GetOffsetEnd(), &alignmentArray[0]);

            cmpFile.alnInfo.alignments[alignmentIndex].StoreAlignmentArray(&alignmentArray[0],
                                                                           alignmentLength);

            /*
             * Read in all additional fields such as quality values, etc..
             */
            std::set<std::string>::iterator fieldIt, fieldEnd;
            fieldEnd = includedFields.end();

            for (fieldIt = includedFields.begin(); fieldIt != fieldEnd; ++fieldIt) {
                if (fieldArray.size() < alignmentLength) {
                    fieldArray.resize(alignmentLength);
                }
                HDFArray<UChar> *fieldArrayPtr = dynamic_cast<HDFArray<UChar> *>(
                    refAlignGroup->readGroups[experimentIndex]->fields[*fieldIt]);

                int ob = cmpFile.alnInfo.alignments[alignmentIndex].GetOffsetBegin();
                int oe = cmpFile.alnInfo.alignments[alignmentIndex].GetOffsetEnd();
                fieldArrayPtr->Read(ob, oe, &fieldArray[0]);

                cmpFile.alnInfo.alignments[alignmentIndex].StoreField(*fieldIt, &fieldArray[0],
                                                                      alignmentLength);
            }
        }
    }

    void IncludeField(std::string fieldName)
    {
        if (supportedFields.find(fieldName) == supportedFields.end()) {
            std::cout << "ERROR, attempting to include field " << fieldName << " that is not supported."
                 << std::endl;
            exit(EXIT_FAILURE);
        }
        includedFields.insert(fieldName);
    }

    template <typename T_Value, typename T_QV>
    void StoreQualityValueFromAlignment(std::vector<T_Value> &fieldValues,
                                        std::vector<int> &baseToAlignmentMap, T_QV *qv)
    {
        for (size_t i = 0; i < baseToAlignmentMap.size(); i++) {
            qv[i] = fieldValues[baseToAlignmentMap[i]];
        }
    }
    /*
       void ReadAlignment(int alignmentIndex, CmpAlignment &cmpAlignment) {
       alnInfoGroup.ReadCmpAlignment(alignmentIndex, cmpAlignment);
       ReadAlignmentArray(alignmentIndex, cmpAlignment.alignmentArray);
       }
       */
    void ReadAlignment(int alignmentIndex, T_Alignment &alignment)
    {
        CmpAlignment cmpAln;
        ReadAlignment(alignmentIndex, cmpAln);

        std::string refSequence;
        std::string readSequence;
        readSequence.resize(cmpAln.alignmentArray.size());
        refSequence.resize(cmpAln.alignmentArray.size());

        ByteAlignmentToQueryString(&cmpAln.alignmentArray[0], cmpAln.alignmentArray.size(),
                                   &readSequence[0]);
        ByteAlignmentToRefString(&cmpAln.alignmentArray[0], cmpAln.alignmentArray.size(),
                                 &refSequence[0]);
        std::string ungappedRead, ungappedRef;
        RemoveGaps(readSequence, ungappedRead);
        RemoveGaps(refSequence, ungappedRef);

        GappedStringsToAlignment(readSequence, refSequence, alignment);
        FASTASequence qAlignedSeq, rAlignedSeq;
        qAlignedSeq.seq = (Nucleotide *)&ungappedRead[0];
        qAlignedSeq.length = ungappedRead.size();
        rAlignedSeq.seq = (Nucleotide *)&ungappedRef[0];
        rAlignedSeq.length = ungappedRef.size();

        alignment.tAlignedSeq.Copy(rAlignedSeq);
        alignment.qAlignedSeq.Copy(qAlignedSeq);

        //unsigned int qStart = cmpAln.GetQueryStart();
        //unsigned int tStart = cmpAln.GetRefStart();

        alignment.tPos = cmpAln.GetRefStart();
        alignment.qPos = cmpAln.GetQueryStart();
        alignment.nIns = cmpAln.GetNInsertions();
        alignment.nDel = cmpAln.GetNDeletions();
        alignment.nMatch = cmpAln.GetNMatch();
        alignment.nMismatch = cmpAln.GetNMismatch();
        alignment.qStrand = 0;
        alignment.tStrand = cmpAln.GetTStrand();
        alignment.pctSimilarity =
            ((float)alignment.nMatch) /
            (alignment.nMatch + alignment.nMismatch + alignment.nIns + alignment.nDel);
        alignment.mapQV = cmpAln.GetMapQV();
    }

    void ReadAlignmentArray(int alignmentIndex, ByteAlignment &alignmentArray)
    {
        CmpAlignment cmpAlignment;
        alnInfoGroup.ReadCmpAlignment(alignmentIndex, cmpAlignment);

        //
        // Cache some stats about the read, and where it was aligned to.
        //
        //int queryStart = cmpAlignment.GetQueryStart();
        //int queryEnd   = cmpAlignment.GetQueryEnd();
        int refGroupId = cmpAlignment.GetRefGroupId();
        int alnGroupId = cmpAlignment.GetAlnGroupId();

        int refGroupIndex = refGroupIdToArrayIndex[refGroupId];
        if (alnGroupIdToReadGroupName.find(alnGroupId) == alnGroupIdToReadGroupName.end()) {
            std::cout << "INTERNAL ERROR! Could not find read group name for alignment "
                 << "group with Id " << alnGroupId << "." << std::endl;
            assert(0);
        }
        std::string readGroupName = alnGroupIdToReadGroupName[alnGroupId];
        if (refAlignGroups[refGroupIndex]->experimentNameToIndex.find(readGroupName) ==
            refAlignGroups[refGroupIndex]->experimentNameToIndex.end()) {
            std::cout << "Internal ERROR! The read group name " << readGroupName
                 << " is specified as part of "
                 << " the path in alignment " << alignmentIndex
                 << " though it does not exist in the ref align group specified for this alignment."
                 << std::endl;
            assert(0);
        }
        int readGroupIndex = refAlignGroups[refGroupIndex]->experimentNameToIndex[readGroupName];

        //HDFCmpExperimentGroup* expGroup = refAlignGroups[refGroupIndex]->readGroups[readGroupIndex];

        int offsetBegin = cmpAlignment.GetOffsetBegin();
        int offsetEnd = cmpAlignment.GetOffsetEnd();

        int alignedSequenceLength = offsetEnd - offsetBegin;

        if (alignedSequenceLength >= 0) {
            alignmentArray.resize(alignedSequenceLength);
        } else {
            return;
        }
        //
        // Read the alignment string.  All alignments
        //
        refAlignGroups[refGroupIndex]->readGroups[readGroupIndex]->alignmentArray.Read(
            offsetBegin, offsetEnd, &alignmentArray[0]);
    }

    void ImportReadFromCmpH5(int alignmentIndex, CmpAlignment &cmpAlignment, SMRTSequence &read)
    {
        alnInfoGroup.ReadCmpAlignment(alignmentIndex, cmpAlignment);

        //
        // Cache some stats about the read, and where it was aligned to.
        //
        //int queryStart = cmpAlignment.GetQueryStart();
        //int queryEnd   = cmpAlignment.GetQueryEnd();
        read.HoleNumber(cmpAlignment.GetHoleNumber());
        int refGroupId = cmpAlignment.GetRefGroupId();
        int alnGroupId = cmpAlignment.GetAlnGroupId();
        int refGroupIndex = refGroupIdToArrayIndex[refGroupId];
        if (alnGroupIdToReadGroupName.find(alnGroupId) == alnGroupIdToReadGroupName.end()) {
            std::cout << "INTERNAL ERROR! Could not find read group name for alignment "
                 << "group with Id " << alnGroupId << "." << std::endl;
            assert(0);
        }
        std::string readGroupName = alnGroupIdToReadGroupName[alnGroupId];

        if (refAlignGroups[refGroupIndex]->experimentNameToIndex.find(readGroupName) ==
            refAlignGroups[refGroupIndex]->experimentNameToIndex.end()) {
            std::cout << "Internal ERROR! The read group name " << readGroupName
                 << " is specified as part of "
                 << " the path in alignment " << alignmentIndex
                 << " though it does not exist in the ref align group specified for this alignment."
                 << std::endl;
            assert(0);
        }

        int readGroupIndex = refAlignGroups[refGroupIndex]->experimentNameToIndex[readGroupName];
        HDFCmpExperimentGroup *expGroup = refAlignGroups[refGroupIndex]->readGroups[readGroupIndex];

        int offsetBegin = cmpAlignment.GetOffsetBegin();
        int offsetEnd = cmpAlignment.GetOffsetEnd();

        int alignedSequenceLength = offsetEnd - offsetBegin;
        std::string alignedSequence;
        std::string readSequence;
        std::vector<unsigned char> byteAlignment;

        if (alignedSequenceLength >= 0) {
            alignedSequence.resize(alignedSequenceLength);
            byteAlignment.resize(alignedSequenceLength);
        }

        //
        // Read the alignment string.  All alignments
        //
        refAlignGroups[refGroupIndex]->readGroups[readGroupIndex]->alignmentArray.Read(
            offsetBegin, offsetEnd, &byteAlignment[0]);

        //
        // Convert to something we can compare easily.
        //
        ByteAlignmentToQueryString(&byteAlignment[0], byteAlignment.size(), &alignedSequence[0]);

        //
        // Initialize the sequence of the read.
        //
        RemoveGaps(alignedSequence, readSequence);

        //
        // Make space for the sequence and all fields.
        //
        read.length = readSequence.size();
        read.Allocate(read.length);
        memcpy(read.seq, readSequence.c_str(), readSequence.size() * sizeof(char));

        std::vector<int> baseToAlignmentMap;
        CreateSequenceToAlignmentMap(byteAlignment, baseToAlignmentMap);

        //
        // Read in the quality values
        //

        std::vector<unsigned char> storedQVArray;

        std::vector<UChar> qvValues;
        std::vector<HalfWord> frameValues;
        int length = offsetEnd - offsetBegin;
        qvValues.resize(length);
        frameValues.resize(length);

        if (expGroup->experimentGroup.ContainsObject("QualityValue")) {
            expGroup->qualityValue.Read(offsetBegin, offsetEnd, &qvValues[0]);
            StoreQualityValueFromAlignment(qvValues, baseToAlignmentMap, &read.qual.data[0]);
            int i;
            for (i = 0; i < read.length; i++) {
                assert(read.qual[i] < 100);
            }
        }

        if (expGroup->experimentGroup.ContainsObject("InsertionQV")) {
            expGroup->insertionQV.Read(offsetBegin, offsetEnd, &qvValues[0]);
            StoreQualityValueFromAlignment(qvValues, baseToAlignmentMap, &read.insertionQV.data[0]);
        }

        if (expGroup->experimentGroup.ContainsObject("SubstitutionQV")) {
            expGroup->substitutionQV.Read(offsetBegin, offsetEnd, &qvValues[0]);
            StoreQualityValueFromAlignment(qvValues, baseToAlignmentMap,
                                           &read.substitutionQV.data[0]);
        }

        if (expGroup->experimentGroup.ContainsObject("DeletionQV")) {
            expGroup->deletionQV.Read(offsetBegin, offsetEnd, &qvValues[0]);
            StoreQualityValueFromAlignment(qvValues, baseToAlignmentMap, &read.deletionQV.data[0]);
        }

        if (expGroup->experimentGroup.ContainsObject("DeletionTag")) {
            std::vector<char> deletionTagValues;
            deletionTagValues.resize(offsetEnd - offsetBegin);
            expGroup->deletionTag.Read(offsetBegin, offsetEnd, &deletionTagValues[0]);
            StoreQualityValueFromAlignment(deletionTagValues, baseToAlignmentMap, read.deletionTag);
        }

        if (expGroup->experimentGroup.ContainsObject("SubstitutionTag")) {
            std::vector<char> substitutionTagValues;
            substitutionTagValues.resize(offsetEnd - offsetBegin);
            expGroup->substitutionTag.Read(offsetBegin, offsetEnd, &substitutionTagValues[0]);
            StoreQualityValueFromAlignment(substitutionTagValues, baseToAlignmentMap,
                                           read.substitutionTag);
        }

        if (expGroup->experimentGroup.ContainsObject("PulseIndex")) {
            std::vector<uint32_t> pulseIndexValues;
            pulseIndexValues.resize(offsetEnd - offsetBegin);
            expGroup->pulseIndex.Read(offsetBegin, offsetEnd, &pulseIndexValues[0]);
            StoreQualityValueFromAlignment(pulseIndexValues, baseToAlignmentMap, read.pulseIndex);
        }

        if (expGroup->experimentGroup.ContainsObject("PreBaseFrames")) {
            expGroup->preBaseFrames.Read(offsetBegin, offsetEnd, &frameValues[0]);
            StoreQualityValueFromAlignment(frameValues, baseToAlignmentMap, read.preBaseFrames);
        }

        if (expGroup->experimentGroup.ContainsObject("WidthInFrames")) {
            expGroup->widthInFrames.Read(offsetBegin, offsetEnd, &frameValues[0]);
            StoreQualityValueFromAlignment(frameValues, baseToAlignmentMap, read.widthInFrames);
        }
    }

    void ReadReadGroupPathTable(CmpIndexedStringTable &readGroupPathTable)
    {
        UInt numElem = readGroupPathTable.ids.size();
        readGroupPathTable.resize(numElem);  // resizes all tables
        readGroupPathIdArray.Read(0, numElem, &readGroupPathTable.ids[0]);
        readGroupPathArray.Read(0, numElem, &readGroupPathTable.names[0]);
        readGroupPathTable.StoreArrayIndexMap();
    }

    void ReadRefSeqTable(CmpIndexedStringTable &refSeqTable)
    {
        UInt numElem = refSeqTable.ids.size();
        refSeqTable.resize(numElem);
        refSeqNameIdArray.Read(0, numElem, &refSeqTable.ids[0]);
        refSeqNameArray.Read(0, numElem, &refSeqTable.names[0]);
        refSeqTable.StoreArrayIndexMap();
    }

    void ReadMovieNameTable(CmpIndexedStringTable &movieTable)
    {
        UInt numElem = movieTable.ids.size();
        movieTable.resize(numElem);
        movieNameIdArray.Read(0, numElem, &movieTable.ids[0]);
        movieNameArray.Read(0, numElem, &movieTable.names[0]);
        movieTable.StoreArrayIndexMap();
    }

    // Store reference FullName, ID, Length and MD5 to /RefInfo
    unsigned int AddRefInfo(std::string refName, unsigned int length, std::string md5)
    {
        return refInfoGroup.AddRefInfo(refName, length, md5);
    }

    unsigned int AddRefGroup(std::string refName, unsigned int refInfoId, std::string &refGroupName)
    {
        if (refInfoId > 999999) {
            // ref000001 ~ ref999999
            std::cout << "ERROR. Could not store more than 999999 references in "
                 << " a cmp.h5 file." << std::endl;
            exit(EXIT_FAILURE);
        }

        // Adding a new refGroup requires:
        // 1. Creating a new refgroup name, e.g., /ref000001.
        // 2. Create the new reference group to add alignments to.
        // 3. Adding this new name to the set of paths of ref groups.
        // 4. Adding information for this ref group.

        // 1.
        GenerateRefGroupName(refInfoId, refGroupName);

        // 2.
        HDFCmpRefAlignmentGroup *newGroup = new HDFCmpRefAlignmentGroup;
        if (newGroup == nullptr) {
            std::cout << "ERROR, unable to allocate memory for cmp.h5 file." << std::endl;
            exit(EXIT_FAILURE);
        }
        newGroup->Create(rootGroup.rootGroup, refGroupName);
        refAlignGroups.push_back(newGroup);

        // 3.
        unsigned int refGroupId = refGroupGroup.AddPath("/" + refGroupName, refInfoId);

        // 4.
        refGroupIdToArrayIndex[refGroupId] = refGroupId - 1;
        refNameToArrayIndex[refName] = refGroupId - 1;

        return refGroupId;
    }

    void GenerateRefGroupName(unsigned int refInfoId, std::string &name)
    {
        // In order to mimic the behaviour of compareSequences,
        // refGroupName should equal to ref00000x, where x is refInfoId.
        // (x used to be refGroupIdToArrayIndex.size() + 1)
        std::stringstream nameStrm;
        nameStrm << "ref";
        nameStrm.width(6);
        nameStrm.fill('0');
        nameStrm << std::right << refInfoId;
        name = nameStrm.str();
    }
};

#endif
