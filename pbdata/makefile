all:

THISDIR:=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))
-include ${CURDIR}/defines.mk
include ${THISDIR}/../rules.mk

CXXOPTS  += -std=c++14 -pedantic -Wall -Wextra
# CURDIR should have LibBlasrConfig.h
#INCLUDES += ${CURDIR}
SYSINCLUDES = ${PBBAM_INC} ${HTSLIB_INC} ${BOOST_INC}
LIBS     += ${PBBAM_LIB} ${HTSLIB_LIB}
LDFLAGS  += $(patsubst %,-L%,${LIBS})

all: static shared
static: libpbdata.a
shared: libpbdata${SH_LIB_EXT}

paths := . matrix reads metagenome qvs saf utils loadpulses alignment amos sam
paths := ${paths} $(patsubst %,${THISDIR}%,${paths})
sources := $(shell find ${THISDIR} -name '*.cpp')
sources := $(notdir ${sources})
objects := $(sources:.cpp=.o)
shared_objects := $(sources:.cpp=.shared.o)
dependencies := $(objects:.o=.d) $(shared_objects:.o=.d)

vpath %.cpp ${paths}

libpbdata.a: $(objects)
	$(AR) $(ARFLAGS) $@ $^

libpbdata${SH_LIB_EXT}: $(shared_objects)

LibBlasrConfig.h:
	echo "Please run 'configure.py' with appropriate args to generate pbdata/LibBlasrConfig.h"

clean: 
	rm -f libpbdata.a  libpbdata.so *.o *.d

-include $(dependencies)
depend: $(dependencies:.d=.depend)
