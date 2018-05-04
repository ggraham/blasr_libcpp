###########
# Sources #
###########

libblasr_sources += files([
  'AcqParams.cpp',
  'BaseFile.cpp',
  'HoleXY.cpp',
  'PulseBaseCommon.cpp',
  'PulseFile.cpp',
  'ReadType.cpp',
  'RegionAnnotation.cpp',
  'RegionAnnotations.cpp',
  'RegionTable.cpp',
  'RegionTypeMap.cpp',
  'ScanData.cpp',
  'ZMWGroupEntry.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'AcqParams.hpp',
    'BaseFile.hpp',
    'BaseFileImpl.hpp',
    'HoleXY.hpp',
    'PulseBaseCommon.hpp',
    'PulseFile.hpp',
    'PulseFileImpl.hpp',
    'ReadInterval.hpp',
    'ReadType.hpp',
    'RegionAnnotation.hpp',
    'RegionAnnotations.hpp',
    'RegionTable.hpp',
    'RegionTypeMap.hpp',
    'ScanData.hpp',
    'ZMWGroupEntry.hpp']),
  subdir : 'libblasr/pbdata/reads')
