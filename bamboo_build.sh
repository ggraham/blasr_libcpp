#!/bin/bash -e
VERBOSE=1 bash -xe scripts/ci/build.sh
#
if [[ $bamboo_planRepository_branchName == master ]]; then
  VERSION=$(git rev-parse --short HEAD)
  NEXUS_URL=http://ossnexus.pacificbiosciences.com/repository/maven-releases/pacbio/sat/blasr_libcpp/$(git rev-parse --short HEAD)
elif [[ $bamboo_planRepository_branchName == develop ]]; then
  VERSION=SNAPSHOT$(git rev-parse --short HEAD)
  NEXUS_URL=http://ossnexus.pacificbiosciences.com/repository/maven-snapshots/pacbio/sat/blasr_libcpp/$(git rev-parse --short HEAD)
else
  VERSION=
  NEXUS_URL=
  exit 0
fi

rm -rf staging
mkdir -p staging/lib staging/include
cp -a build/liblibcpp.a staging/lib
find alignment hdf pbdata -name '*.hpp' -o -name '*.h' | \
xargs tar cf - | tar xf - -C staging/include

cd staging

tar zcf ../blasr_libcpp-${VERSION}.tgz lib include
md5sum ../blasr_libcpp-${VERSION}.tgz | awk -e '{print $1}' >| ../blasr_libcpp-${VERSION}.tgz.md5
sha1sum ../blasr_libcpp-${VERSION}.tgz | awk -e '{print $1}' >| ../blasr_libcpp-${VERSION}.tgz.sha1

curl -vn --upload-file ../blasr_libcpp-${VERSION}.tgz \
  $NEXUS_URL/blasr_libcpp-${VERSION}.tgz
curl -vn --upload-file ../blasr_libcpp-${VERSION}.tgz.md5 \
  $NEXUS_URL/blasr_libcpp-${VERSION}.tgz.md5
curl -vn --upload-file ../blasr_libcpp-${VERSION}.tgz.sha1 \
  $NEXUS_URL/blasr_libcpp-${VERSION}.tgz.sha1
