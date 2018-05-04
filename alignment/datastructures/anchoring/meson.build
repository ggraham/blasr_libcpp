###########
# Sources #
###########

libblasr_sources += files([
  'AnchorParameters.cpp',
  'ClusterList.cpp',
  'MatchPos.cpp',
  'WeightedInterval.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'AnchorParameters.hpp',
    'ClusterList.hpp',
    'MatchPos.hpp',
    'WeightedInterval.hpp']),
  subdir : 'libblasr/alignment/datastructures/anchoring')
