[![Build Status](https://travis-ci.org/PacificBiosciences/blasr_libcpp.svg?branch=master)](https://travis-ci.org/PacificBiosciences/blasr_libcpp)

#What is blasr_libcpp#

**Blasr_libcpp** is a *library* used by **blasr** and other executables such as samtoh5, loadPulses for analyzing PacBio sequences. This library contains three sub-libraries, including pbdata, hdf and alignment:
+ pbdata
   - contains source code for handling Pacbio sequences and can build lib ```libpbdata```.
+ hdf
   - contains source code for handling Pacbio hdf5 files (e.g., *.pls.h5, *.rgn.h5, *.bas.h5) and builds ```libpbhdf```.
+ alignment
   - contains source code for aligning Pacbio reads to target sequences used in blasr and builds ```libblasr```.

For more information, see
* https://github.com/PacificBiosciences/blasr_libcpp/wiki

## Building using make
The simplest way is:
```
NOPBBAM=1 ./configure.py
make -j all
```
That will skip pbbam, and it will download HDF5 headers.

## Building using cmake

Make sure that you are using cmake >=3.7 and
always start from an empty build subdirectory!

    git clone git://github.com/PacificBiosciences/blasr_libcpp.git && cd blasr_libcpp
    git submodule update --init --remote
    mkdir build && cd build
    cmake -GNinja .. && ninja check_libcpp

Is your HDF5 in a custom location?

    cmake -GNinja -DHDF5_ROOT=/your/location/hdf-1.8.16 ..

Are HDF$ libraries and include folders in different locations?

    cmake -GNinja -DHDF5_LIBRARIES=/your/location/hdf-1.8.16/lib
                  -DHDF5_INCLUDE_DIRS=/other/location/hdf-1.8.16/include ..

Prefer a custom libz implementation?

    cmake -GNinja -DZLIB_INCLUDE_DIRS=/your/location/zlib/include \
                  -DZLIB_LIBRARIES=/your/location/zlib/libz.so ..

DISCLAIMER
----------
THIS WEBSITE AND CONTENT AND ALL SITE-RELATED SERVICES, INCLUDING ANY DATA, ARE PROVIDED "AS IS," WITH ALL FAULTS, WITH NO REPRESENTATIONS OR WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTIES OF MERCHANTABILITY, SATISFACTORY QUALITY, NON-INFRINGEMENT OR FITNESS FOR A PARTICULAR PURPOSE. YOU ASSUME TOTAL RESPONSIBILITY AND RISK FOR YOUR USE OF THIS SITE, ALL SITE-RELATED SERVICES, AND ANY THIRD PARTY WEBSITES OR APPLICATIONS. NO ORAL OR WRITTEN INFORMATION OR ADVICE SHALL CREATE A WARRANTY OF ANY KIND. ANY REFERENCES TO SPECIFIC PRODUCTS OR SERVICES ON THE WEBSITES DO NOT CONSTITUTE OR IMPLY A RECOMMENDATION OR ENDORSEMENT BY PACIFIC BIOSCIENCES.
