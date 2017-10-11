#!/bin/bash -e
DISTFILES_URL=http://nexus/repository/unsupported/distfiles
curl -sL $DISTFILES_URL/googletest/release-1.8.0.tar.gz \
| tar zxf - --strip-components 1 -C googletest
VERBOSE=1 bash -xe scripts/ci/build.sh
#
rm -rf staging
mkdir -p staging/lib staging/include
cp -a build/liblibcpp.a staging/lib
find alignment hdf pbdata -name '*.hpp' -o -name '*.h' | \
xargs tar cf - | tar xf - -C staging/include
cd staging
tar zcf ../blasr_libcpp-SNAPSHOT.tgz lib include
md5sum ../blasr_libcpp-SNAPSHOT.tgz | awk -e '{print $1}' >| ../blasr_libcpp-SNAPSHOT.tgz.md5
sha1sum ../blasr_libcpp-SNAPSHOT.tgz | awk -e '{print $1}' >| ../blasr_libcpp-SNAPSHOT.tgz.sha1
UNSUPPORTED_URL=http://ossnexus.pacificbiosciences.com/repository/unsupported
curl -vn --upload-file ../blasr_libcpp-SNAPSHOT.tgz \
$UNSUPPORTED_URL/gcc-6.4.0/blasr_libcpp-SNAPSHOT.tgz
curl -vn --upload-file ../blasr_libcpp-SNAPSHOT.tgz.md5 \
$UNSUPPORTED_URL/gcc-6.4.0/blasr_libcpp-SNAPSHOT.tgz.md5
curl -vn --upload-file ../blasr_libcpp-SNAPSHOT.tgz.sha1 \
$UNSUPPORTED_URL/gcc-6.4.0/blasr_libcpp-SNAPSHOT.tgz.sha1
