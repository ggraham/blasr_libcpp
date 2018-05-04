###########
# Sources #
###########

libblasr_sources += files([
  'BlasrFileUtils.cpp',
  'FileOfFileNames.cpp',
  'LogUtils.cpp',
  'PhredUtils.cpp',
  'RangeUtils.cpp',
  'RegionUtils.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'FileOfFileNames.hpp',
    'FileUtils.hpp',
    'LogUtils.hpp',
    'PhredUtils.hpp',
    'RangeUtils.hpp',
    'RegionUtils.hpp',
    'RegionUtilsImpl.hpp',
    'SimpleXMLUtils.hpp']),
  subdir : 'libblasr/alignment/utils')
