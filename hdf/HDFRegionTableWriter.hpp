#ifndef _BLASR_HDF_REGION_TABLE_WRITER_HPP_
#define _BLASR_HDF_REGION_TABLE_WRITER_HPP_

#include <string>
// pbdata/
#include "../pbdata/Enumerations.h"
#include "../pbdata/reads/RegionTable.hpp"
#include "HDF2DArray.hpp"
#include "HDFArray.hpp"
#include "HDFAtom.hpp"
#include "HDFFile.hpp"

using namespace H5;

class HDFRegionTableWriter
{
public:
    HDFFile regionTableFile;

    HDF2DArray<int> regions;
    HDFAtom<std::vector<std::string> > regionTypes;
    HDFAtom<std::vector<std::string> > regionDescriptions;
    HDFAtom<std::vector<std::string> > regionSources;
    HDFAtom<std::vector<std::string> > columnNames;
    int curRow;
    int nRows;
    HDFGroup *parentGroupPtr, pulseDataGroup;

    void CreateGroupStructure()
    {

        regionTableFile.rootGroup.AddGroup("PulseData");
        if (pulseDataGroup.Initialize(regionTableFile.rootGroup, "PulseData") == 0) {
            std::cout << "Could not create group PulseData. This is a bug." << std::endl;
            std::exit(EXIT_FAILURE);
        }
        parentGroupPtr = &pulseDataGroup;
        regions.Initialize(pulseDataGroup, "Regions", RegionAnnotation::NCOLS);
    }

    int Create(std::string fileName)
    {
        H5File newFile(fileName.c_str(), H5F_ACC_TRUNC, FileCreatPropList::DEFAULT,
                       FileAccPropList::DEFAULT);
        regionTableFile.hdfFile.openFile(fileName.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        regionTableFile.rootGroup.Initialize(regionTableFile.hdfFile, "/");
        CreateGroupStructure();
    }

    HDFRegionTableWriter()
    {
        curRow = 0;
        nRows = 0;
        parentGroupPtr = NULL;
    }

    int Initialize(HDFGroup &parentGroupP)
    {
        /*
         * Initialize in an existing file.
         */
        parentGroupPtr = &parentGroupP;
        return Initialize();
    }

    int Initialize(std::string &regionTableFileName,
                   const H5::FileAccPropList &fileAccPropList = H5::FileAccPropList::DEFAULT)
    {
        /*
         * Initialize access to the HDF file.
         */
        try {
            regionTableFile.Open(regionTableFileName, H5F_ACC_TRUNC, fileAccPropList);
        } catch (Exception &e) {
            std::cout << e.getDetailMsg() << std::endl;
            return 0;
        }
        CreateGroupStructure();
        return 1;
    }

    int Initialize()
    {

        if (regions.Initialize(*parentGroupPtr, "Regions", RegionAnnotation::NCOLS) == 0) {
            return 0;
        }
        nRows = regions.GetNRows();

        curRow = 0;
        return 1;
    }

    void Finalize(std::vector<std::string> &columnNamesVect, std::vector<std::string> &regionTypesVect,
                  std::vector<std::string> &regionDescriptionsVect, std::vector<std::string> &regionSourcesVect)
    {
        //
        // Make sure data has been written to the dataset.  If not, the
        // dataset will not exist and creating the attributes will fail.
        //
        if (curRow > 0) {
            regionTypes.Create(regions.dataset, "RegionTypes", regionTypesVect);
            columnNames.Create(regions.dataset, "ColumnNames", columnNamesVect);
            regionDescriptions.Create(regions.dataset, "RegionDescriptions",
                                      regionDescriptionsVect);
            regionSources.Create(regions.dataset, "RegionSources", regionSourcesVect);
        }
    }

    void WriteRows(std::vector<RegionAnnotation> &annotations)
    {
        int i;
        for (i = 0; i < annotations.size(); i++) {
            Write(annotations[i]);
        }
    }

    int Write(RegionAnnotation &annotation, int at = -1)
    {
        if (at == -1) {
            regions.WriteRow(annotation.row, annotation.NCOLS);
            ++curRow;
        } else {
        }
        return 1;
    }

    void Close()
    {
        regions.Close();
        //
        // Check to see if this is external
        //
        if (parentGroupPtr == &pulseDataGroup) {
            pulseDataGroup.Close();
        }
        regionTableFile.rootGroup.Close();
    }
};

#endif
