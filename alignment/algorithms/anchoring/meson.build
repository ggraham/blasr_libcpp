###########
# Sources #
###########

libblasr_sources += files([
  'BWTSearch.cpp',
  'ClusterProbability.cpp',
  'Coordinate.cpp',
  'FindMaxInterval.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'BasicEndpoint.hpp',
    'BasicEndpointImpl.hpp',
    'BWTSearch.hpp',
    'BWTSearchImpl.hpp',
    'ClusterProbability.hpp',
    'Coordinate.hpp',
    'FindMaxInterval.hpp',
    'FindMaxIntervalImpl.hpp',
    'GlobalChain.hpp',
    'GlobalChainImpl.hpp',
    'LISPValue.hpp',
    'LISPValueImpl.hpp',
    'LISPValueWeightor.hpp',
    'LISPValueWeightorImpl.hpp',
    'LISQValueWeightor.hpp',
    'LISSizeWeightor.hpp',
    'LISSizeWeightorImpl.hpp',
    'LongestIncreasingSubsequence.hpp',
    'LongestIncreasingSubsequenceImpl.hpp',
    'MapBySuffixArray.hpp',
    'MapBySuffixArrayImpl.hpp',
    'PrioritySearchTree.hpp',
    'PrioritySearchTreeImpl.hpp',
    'ScoreAnchors.hpp',
    'ScoreAnchorsImpl.hpp']),
  subdir : 'libblasr/alignment/algorithms/anchoring')
