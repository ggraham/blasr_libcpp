###########
# Sources #
###########

libblasr_sources += files([
  'DifferenceCovers.cpp',
  'LightweightSuffixArray.cpp',
  'MultikeyQuicksort.cpp',
  'qsufsort.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'DifferenceCovers.hpp',
    'Karkkainen.hpp',
    'LightweightSuffixArray.hpp',
    'MultikeyQuicksort.hpp',
    'qsufsort.hpp']),
  subdir : 'libblasr/alignment/algorithms/sorting')
