###########
# Sources #
###########

libblasr_sources += files([
  'SDPFragment.cpp',
  'SparseDynamicProgramming.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'FragmentSort.hpp',
    'FragmentSortImpl.hpp',
    'NonoverlappingSparseDynamicProgramming.h',
    'SDPColumn.hpp',
    'SDPFragment.hpp',
    'SDPSet.hpp',
    'SDPSetImpl.hpp',
    'SparseDynamicProgramming.hpp',
    'SparseDynamicProgrammingImpl.hpp',
    'VariableLengthSDPFragment.h']),
  subdir : 'libblasr/alignment/algorithms/alignment/sdp')
