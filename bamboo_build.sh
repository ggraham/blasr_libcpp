#!/usr/bin/env bash
set -vex

################
# DEPENDENCIES #
################

## Load modules
type module >& /dev/null || . /mnt/software/Modules/current/init/bash

module purge

module load gcc
module load ccache

module load meson
module load ninja

module load boost
module load gtest

case "${bamboo_planRepository_branchName}" in
  master)
    module load pbbam/master
    ;;
  *)
    module load pbbam/develop
    ;;
esac


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

case "${bamboo_planRepository_branchName}" in
  develop|master)
    export PREFIX_ARG="/mnt/software/l/libblasr/${bamboo_planRepository_branchName}"
    export BUILD_NUMBER="${bamboo_globalBuildNumber:-0}"
    ;;
  *)
    export BUILD_NUMBER="0"
    ;;
esac

# call the main build+test scripts
export CURRENT_BUILD_DIR="build"
export ENABLED_TESTS="true"

# i : HDF5 major version
for i in "1.10" "1.8"; do
  module load hdf5-tools/${i}

  CURRENT_BUILD_DIR="build_hdf5=${i}"

  # TODO(dseifert)
  # HDF5 doesn't have pkg-config files yet
  export CPPFLAGS="${HDF5_CFLAGS}"
  export LDFLAGS="-static-libstdc++ -static-libgcc ${HDF5_LIBS}"

  bash scripts/ci/build.sh
  bash scripts/ci/test.sh

  module unload hdf5-tools
done

# create symlink so Bamboo can find the xunit output
ln -s "${CURRENT_BUILD_DIR}" build

if [[ -z ${PREFIX_ARG+x} ]]; then
  echo "Not installing anything (branch: ${bamboo_planRepository_branchName}), exiting."
  exit 0
fi

bash scripts/ci/install.sh
