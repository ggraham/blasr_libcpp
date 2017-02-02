
include(CheckCXXCompilerFlag)

# shared CXX flags for all source code & tests
SET_PROPERTY(GLOBAL PROPERTY BLASRLIBCPP_COMPIPLE_FLAGS_GLOBAL "-std=c++11 -pedantic -g -Wno-long-long -Wall -Wextra -Wno-return-type -Wno-overloaded-virtual -Wno-unused-parameter -Wno-div-by-zero -Wno-unused-variable -Wno-unused-local-typedefs  -DUSE_PBBAM")

# static linking
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(BLASRLIBCPP_LINKER_FLAGS "${BLASRLIBCPP_LINKER_FLAGS} -static-libstdc++")
ENDIF()

if (HDF5_INCLUDE_DIR)
    message(FATAL_ERROR "Please specify HDF5_INCLUDE_DIRS not HDF5_INCLUDE_DIR!")
endif()
if (HDF5_LIBRARY)
    message(FATAL_ERROR "Please specify HDF5_LIBRARIES not HDF5_LIBRARY!")
endif()

if (NOT HDF5_INCLUDE_DIRS OR NOT HDF5_LIBRARIES)
    find_package(HDF5 REQUIRED)
    if (HDF5_ROOT)
        SET_PROPERTY(GLOBAL PROPERTY HDF5_LINKER_FLAG_GLOBAL "-L${HDF5_ROOT}/lib")
    endif()
else()
    SET_PROPERTY(GLOBAL PROPERTY HDF5_LINKER_FLAG_GLOBAL "-L${HDF5_LIBRARIES}")
    find_library(HDF5_LIBRARIES_     hdf5    ${HDF5_LIBRARIES} NO_CMAKE_SYSTEM_PATH)
    find_library(HDF5_CPP_LIBRARIES hdf5_cpp ${HDF5_LIBRARIES} NO_CMAKE_SYSTEM_PATH)
endif()

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(BLASRLIBCPP_LINKER_FLAGS "${BLASRLIBCPP_LINKER_FLAGS} -lrt")
endif()

SET_PROPERTY(GLOBAL PROPERTY BLASRLIBCPP_LINK_FLAGS_GLOBAL ${BLASRLIBCPP_LINKER_FLAGS})