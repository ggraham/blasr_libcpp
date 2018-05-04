###########
# Sources #
###########

libblasr_sources += files([
  'ContextSample.cpp',
  'ContextSet.cpp',
  'LengthHistogram.cpp',
  'QualitySample.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'CDFMap.hpp',
    'ContextOutputList.hpp',
    'ContextSample.hpp',
    'ContextSet.hpp',
    'LengthHistogram.hpp',
    'OutputList.hpp',
    'OutputSample.hpp',
    'OutputSampleList.hpp',
    'OutputSampleListSet.hpp',
    'QualitySample.hpp']),
  subdir : 'libblasr/alignment/simulator')
