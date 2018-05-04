###########
# Sources #
###########

libblasr_sources += files([
  'QualityValueProfile.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files(['QualityValueProfile.hpp']),
  subdir : 'libblasr/alignment/qvs')
