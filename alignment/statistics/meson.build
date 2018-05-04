###########
# Sources #
###########

libblasr_sources += files([
  'cdfs.cpp',
  'LookupAnchorDistribution.cpp',
  'pdfs.cpp',
  'StatUtils.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'AnchorDistributionTable.hpp',
    'cdfs.hpp',
    'LookupAnchorDistribution.hpp',
    'pdfs.hpp',
    'StatUtils.hpp',
    'StatUtilsImpl.hpp',
    'VarianceAccumulator.hpp',
    'VarianceAccumulatorImpl.hpp']),
  subdir : 'libblasr/alignment/statistics')
