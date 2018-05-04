#!/usr/bin/env bash
set -vex

echo "################"
echo "# DEPENDENCIES #"
echo "################"

echo "## Load modules"
type module >& /dev/null || . /mnt/software/Modules/current/init/bash

set +vx
module purge

module load gcc
module load ccache

module load meson
module load ninja

module load boost
module load gtest

module load pbbam

set -vx

BOOST_ROOT="${BOOST_ROOT%/include}"
# unset these variables to have meson discover all
# boost-dependent variables from BOOST_ROOT alone
unset BOOST_INCLUDEDIR
unset BOOST_LIBRARYDIR

export CC="ccache gcc"
export CXX="ccache g++"
export CCACHE_BASEDIR="${PWD}"

if [[ $USER == bamboo ]]; then
  export CCACHE_DIR=/mnt/secondary/Share/tmp/bamboo.${bamboo_shortPlanKey}.ccachedir
  export CCACHE_TEMPDIR=/scratch/bamboo.ccache_tempdir
fi


echo "#########"
echo "# BUILD #"
echo "#########"

case "${bamboo_planRepository_branchName}" in
  develop|master)
    PREFIX_ARG="/mnt/software/l/libblasr/${bamboo_planRepository_branchName}"
    BUILD_NUMBER="${bamboo_globalBuildNumber:-0}"
    ;;
  *)
    BUILD_NUMBER="0"
    ;;
esac

# in order to make shared libraries consumable
# by conda and other package managers
default_LDFLAGS="-static-libstdc++ -static-libgcc"

# i : HDF5 major version
for i in "1.8"; do
  module load hdf5-tools/${i}

  CURRENT_BUILD_DIR="build_hdf5=${i}"
  mkdir -p "${CURRENT_BUILD_DIR}"/test-reports

  echo "=============================="
  echo "Current configuration:"
  echo "   HDF5:             ${i}"
  echo "=============================="

  # 1. configure
  # '--wrap-mode nofallback' prevents meson from downloading
  # stuff from the internet or using subprojects.
  echo "## Configuring source (${CURRENT_BUILD_DIR})"
  CPPFLAGS="${HDF5_CFLAGS}" \
  LDFLAGS="${default_LDFLAGS} ${HDF5_LIBS}" \
    meson \
      --wrap-mode nofallback \
      --backend ninja \
      --buildtype release \
      --strip \
      --default-library shared \
      --libdir lib \
      --prefix "${PREFIX_ARG:-/usr/local}" \
      -Dtests=true \
      "${CURRENT_BUILD_DIR}" .

  # 2. build
  echo "## Building source (${CURRENT_BUILD_DIR})"
  ninja -C "${CURRENT_BUILD_DIR}" -v

  # 3. tests
  echo "## Tests (${CURRENT_BUILD_DIR})"
  GTEST_OUTPUT="xml:${CURRENT_BUILD_DIR}/test-reports/libblasr_results.xml" ARGS=-V VERBOSE=1 \
  ninja -C "${CURRENT_BUILD_DIR}" -v test

  module unload hdf5-tools
done

if [[ -z ${PREFIX_ARG+x} ]]; then
  echo "Not installing anything (branch: ${bamboo_planRepository_branchName}), exiting."
  exit 0
fi

echo "###########"
echo "# INSTALL #"
echo "###########"

echo "## Cleaning out old installation from /mnt/software"
rm -rf "${PREFIX_ARG}"/*

echo "## Installing to /mnt/software"
ninja -C "${CURRENT_BUILD_DIR}" -v install
