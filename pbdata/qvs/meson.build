###########
# Sources #
###########

libblasr_sources += files([
  'BlasrQualityValue.cpp',
  'QualityTransform.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'QualityTransform.hpp',
    'QualityValue.hpp',
    'QualityValueVector.hpp',
    'QualityValueVectorImpl.hpp',
    'QualityTransform.hpp',
    'QualityValue.hpp',
    'QualityValueVector.hpp',
    'QualityValueVectorImpl.hpp']),
  subdir : 'libblasr/pbdata/qvs')
