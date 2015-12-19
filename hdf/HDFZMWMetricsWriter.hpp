// Author: Yuan Li


#ifndef _BLASR_HDF_HDFZMWMETRICSWriter_HPP_
#define _BLASR_HDF_HDFZMWMETRICSWriter_HPP_

#include "SMRTSequence.hpp"
#include "HDFWriterBase.hpp"
#include "BufferedHDFArray.hpp"
#include "BufferedHDF2DArray.hpp"


class HDFBaseCallerWriter;

class HDFZMWMetricsWriter: public HDFWriterBase {

friend class HDFBaseCallerWriter;
private:
    /// \name Private variable
    /// \{
    HDFGroup & parentGroup_;
    
	HDFGroup zmwMetricsGroup_;

    /// HDF2DArray for writing average SNR within HQRegion.
   	BufferedHDF2DArray<float> hqRegionSNRArray_;

    /// HDFArray for writing read raw accuracy prediction.
   	BufferedHDFArray<float> readScoreArray_;

    /// HDFArray for writing Productivity
    BufferedHDFArray<unsigned char> productivityArray_;

    /// Map bases (e.g., ACGT) to indices
    std::map<char, size_t> baseMap_;

    int curRow_;

    static const int SNRNCOLS = 4;
    /// \}

public:
    /// \name Constructors and Destructors
    /// \{
    HDFZMWMetricsWriter(const std::string & filename, 
                        HDFGroup & parentGroup,
                        const std::map<char, size_t> & baseMap);

    ~HDFZMWMetricsWriter(void) ;
    /// \}

    /// \name Public Methods
    /// \{
    
    /// \note Write info of a SMRTSequence to ZMWMetrics,
    ///       (1) add average signal to noise ratio in HQRegion to HQRegionSNR 
    ///       (2) add read raw accuracy prediction to ReadScore 
    bool WriteOneZmw(const SMRTSequence & read);


    /// \note Flushes all data from cache to disc.
    void Flush(void);

    /// \note Closes this zmw group as well as child hdf groups.
    void Close(void);
    /// \}

private:
    /// \name Private Methods
    /// \{
    
    /// \note Initialize child hdf groups under ZMWMetrics, including
    ///       HQRegionSNR and ReadScore 
    /// \reutrns bool, whether or not child hdf groups successfully initialized.
    bool InitializeChildHDFGroups(void);

    /// \note Write Attributes.
    bool WriteAttributes(void);
    /// \}
};

#endif
