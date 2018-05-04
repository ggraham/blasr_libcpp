###########
# Sources #
###########

libblasr_sources += files(['CmpAlignment.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'CmpAlignment.hpp',
    'CmpAlignmentImpl.hpp']),
  subdir : 'libblasr/pbdata/alignment')
