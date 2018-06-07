# What is blasr_libcpp

**Blasr_libcpp** is a *library* used by **blasr** and other executables such as samtoh5, loadPulses for analyzing PacBio sequences. This library contains three sub-libraries, including pbdata, hdf and alignment:
+ pbdata
   - contains source code for handling Pacbio sequences and can build lib ```libpbdata```.
+ hdf
   - contains source code for handling Pacbio hdf5 files (e.g., *.pls.h5, *.rgn.h5, *.bas.h5) and builds ```libpbhdf```.
+ alignment
   - contains source code for aligning Pacbio reads to target sequences used in blasr and builds ```libblasr```.

## Availability
The latest release binary can be installed via [bioconda](https://bioconda.github.io/):

    conda install -c bioconda blasr_libcpp

Official support is only provided for official and stable
[SMRT Analysis builds](http://www.pacb.com/products-and-services/analytical-software/)
provided by PacBio and not for source builds.

Unofficial support for binary releases is provided via github issues,
not via mail to developers.
**No** support for source builds; issues will be closed without comment.

## Documentation

 - [Wiki: all you need to know](https://github.com/PacificBiosciences/blasr_libcpp/wiki)
 - [Developer environment](DEVELOPER.md)

DISCLAIMER
----------
THIS WEBSITE AND CONTENT AND ALL SITE-RELATED SERVICES, INCLUDING ANY DATA, ARE PROVIDED "AS IS," WITH ALL FAULTS, WITH NO REPRESENTATIONS OR WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTIES OF MERCHANTABILITY, SATISFACTORY QUALITY, NON-INFRINGEMENT OR FITNESS FOR A PARTICULAR PURPOSE. YOU ASSUME TOTAL RESPONSIBILITY AND RISK FOR YOUR USE OF THIS SITE, ALL SITE-RELATED SERVICES, AND ANY THIRD PARTY WEBSITES OR APPLICATIONS. NO ORAL OR WRITTEN INFORMATION OR ADVICE SHALL CREATE A WARRANTY OF ANY KIND. ANY REFERENCES TO SPECIFIC PRODUCTS OR SERVICES ON THE WEBSITES DO NOT CONSTITUTE OR IMPLY A RECOMMENDATION OR ENDORSEMENT BY PACIFIC BIOSCIENCES.
