<h1 align="center"><img src="http://www.pacb.com/wp-content/themes/pacific-biosciences/img/pacific-biosciences-logo-mobile.svg"/></h1>
<h1 align="center">blasr_libcpp</h1>
<p align="center">A C++ library for blasr</p>

***

## Availability
This library is part of blasr and latest version can be installed via bioconda package `blasr`.

Please refer to our [official pbbioconda page](https://github.com/PacificBiosciences/pbbioconda)
for information on Installation, Support, License, Copyright, and Disclaimer.

# What is blasr_libcpp

**Blasr_libcpp** is a *library* used by **blasr** and other executables such as samtoh5, loadPulses for analyzing PacBio sequences. This library contains three sub-libraries, including pbdata, hdf and alignment:
+ pbdata
   - contains source code for handling Pacbio sequences and can build lib ```libpbdata```.
+ hdf
   - contains source code for handling Pacbio hdf5 files (e.g., *.pls.h5, *.rgn.h5, *.bas.h5) and builds ```libpbhdf```.
+ alignment
   - contains source code for aligning Pacbio reads to target sequences used in blasr and builds ```libblasr```.

DISCLAIMER
----------
THIS WEBSITE AND CONTENT AND ALL SITE-RELATED SERVICES, INCLUDING ANY DATA, ARE PROVIDED "AS IS," WITH ALL FAULTS, WITH NO REPRESENTATIONS OR WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTIES OF MERCHANTABILITY, SATISFACTORY QUALITY, NON-INFRINGEMENT OR FITNESS FOR A PARTICULAR PURPOSE. YOU ASSUME TOTAL RESPONSIBILITY AND RISK FOR YOUR USE OF THIS SITE, ALL SITE-RELATED SERVICES, AND ANY THIRD PARTY WEBSITES OR APPLICATIONS. NO ORAL OR WRITTEN INFORMATION OR ADVICE SHALL CREATE A WARRANTY OF ANY KIND. ANY REFERENCES TO SPECIFIC PRODUCTS OR SERVICES ON THE WEBSITES DO NOT CONSTITUTE OR IMPLY A RECOMMENDATION OR ENDORSEMENT BY PACIFIC BIOSCIENCES.
