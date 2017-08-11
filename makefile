SHELL=/bin/bash

THISDIR:=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))

.PHONY: all libpbdata libhdf libblasr gtest clean cleanall

all:
	${MAKE} libpbdata
	${MAKE} libpbihdf
	${MAKE} libblasr
all-debug:
	${MAKE} CXXFLAGS=-g all
all-opt:
	${MAKE} CXXFLAGS=-O3 all
all-depend:
	${MAKE} -C ${THISDIR}/pbdata depend
libpbdata:
	${MAKE} -C ${THISDIR}/pbdata LibBlasrConfig.h
	${MAKE} -C ${THISDIR}/pbdata all
libpbihdf:
	${MAKE} -C ${THISDIR}/hdf all
libblasr:
	${MAKE} -C ${THISDIR}/alignment all
install-includes: # Into ${BLASR_INC}, e.g. ${PREFIX}/include/blasr/
	rsync -a --files-from=${THISDIR}/exports ${THISDIR} ${BLASR_INC}
gtest:
	${MAKE} -C ${THISDIR}/unittest gtest
clean:
	${MAKE} -C ${THISDIR}/pbdata clean
	${MAKE} -C ${THISDIR}/hdf clean
	${MAKE} -C ${THISDIR}/alignment clean
	${MAKE} -C ${THISDIR}/unittest clean
cleanall: clean
