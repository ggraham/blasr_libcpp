#!/bin/bash
set -euo pipefail

echo "#############################"
echo "# LOAD MODULES"
source /mnt/software/Modules/current/init/bash
module load git gcc ccache boost ninja cmake hdf5-tools zlib htslib
export CCACHE_COMPILERCHECK='%compiler% -dumpversion'
export CCACHE_DIR=/mnt/secondary/Share/tmp/bamboo.mobs.ccachedir
export CCACHE_BASEDIR=$PWD

echo "#############################"
echo "# PRE-BUILD HOOK"
echo "## Check formatting"
DISTFILES_URL=http://nexus/repository/unsupported/distfiles
curl -sL $DISTFILES_URL/googletest/release-1.8.0.tar.gz \
| tar zxf - --strip-components 1 -C googletest
rm -rf pbbam
for myfile in $PWD/pbbam-*gz; do
  tar zxvf $myfile
  mydir=$(echo $myfile|sed -e 's|-x86_64.tgz||')
  PacBioBAM_INCLUDE_DIRS="$mydir/include"
  PacBioBAM_LIBRARIES="$mydir/lib/libpbbam.a"
  PacBioBAM_LIB="$mydir/lib"
  break
done
ln -sfn $(pkg-config --cflags-only-I htslib|sed -e 's@-I@@;s@ @@g')/htslib $PacBioBAM_INCLUDE_DIRS/htslib
sed -i -e "s|target_link_libraries(libcpp \${PacBioBAM_LIBRARIES}|target_link_libraries(libcpp \${PacBioBAM_LIBRARIES} $(pkg-config --libs htslib) |" \
CMakeLists.txt
grep ^target_link_libraries CMakeLists.txt

echo "#############################"
echo "# BUILD"
echo "## Create build directory "
mkdir -p build
echo "## Build source"
( cd build &&\
  rm -rf * &&\
  CMAKE_BUILD_TYPE=ReleaseWithAssert cmake -GNinja \
    -DHDF5_ROOT=$HDF5_DIR \
    -DPacBioBAM_INCLUDE_DIRS=$PacBioBAM_INCLUDE_DIRS \
    -DPacBioBAM_LIBRARIES=$PacBioBAM_LIBRARIES \
  .. &&\
  ninja )

echo "#############################"
echo "# TEST"
echo "## ctest"
( cd build && ninja check_libcpp )
