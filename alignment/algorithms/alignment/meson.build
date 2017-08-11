##################
# Subdirectories #
##################

subdir('sdp')

###########
# Sources #
###########

libblasr_sources += files([
  'AlignmentUtils.cpp',
  'BaseScoreFunction.cpp',
  'ExtendAlign.cpp',
  'GuidedAlign.cpp',
  'IDSScoreFunction.cpp',
  'KBandAlign.cpp',
  'QualityValueScoreFunction.cpp',
  'ScoreMatrices.cpp',
  'StringToScoreMatrix.cpp'])

###########
# Headers #
###########

install_headers(
  files([
    'AffineGuidedAlign.hpp',
    'AffineKBandAlign.hpp',
    'AlignmentFormats.hpp',
    'AlignmentUtils.hpp',
    'AlignmentUtilsImpl.hpp',
    'BaseScoreFunction.hpp',
    'DistanceMatrixScoreFunction.hpp',
    'DistanceMatrixScoreFunctionImpl.hpp',
    'ExtendAlign.hpp',
    'FullQVAlign.hpp',
    'GraphPaper.hpp',
    'GraphPaperImpl.hpp',
    'GuidedAlign.hpp',
    'IDSScoreFunction.hpp',
    'KBandAlign.hpp',
    'OneGapAlignment.hpp',
    'QualityValueScoreFunction.hpp',
    'ScoreMatrices.hpp',
    'SDPAlign.hpp',
    'SDPAlignImpl.hpp',
    'StringToScoreMatrix.hpp',
    'SWAlign.hpp',
    'SWAlignImpl.hpp']),
  subdir : 'libblasr/alignment/algorithms/alignment')
