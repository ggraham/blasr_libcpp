#!/bin/bash
set -euo pipefail

echo "#############################"
echo "# LOAD MODULES"
source /mnt/software/Modules/current/init/bash
module load git gcc/6.4.0 ccache boost ninja cmake hdf5-tools zlib htslib
export CCACHE_COMPILERCHECK='%compiler% -dumpversion'
export CCACHE_DIR=/mnt/secondary/Share/tmp/bamboo.mobs.ccachedir
export CCACHE_BASEDIR=$PWD

echo "#############################"
echo "# PRE-BUILD HOOK"
echo "## Check formatting"
# ./tools/check-formatting --all

echo "#############################"
echo "# BUILD"
echo "## Create build directory "
if [ ! -d build ] ; then mkdir build ; fi

echo "## Build source"
( cd build &&\
  rm -rf * &&\
  CMAKE_BUILD_TYPE=ReleaseWithAssert cmake -GNinja -DHDF5_ROOT=$HDF5_DIR .. &&\
  ninja )

echo "#############################"
echo "# TEST"
echo "## ctest"
( cd build && ninja check_libcpp )
