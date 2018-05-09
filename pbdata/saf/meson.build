###########
# Sources #
###########

libblasr_sources += files([
  'AlnGroup.cpp',
  'MovieInfo.cpp',
  'RefGroup.cpp',
  'RefInfo.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'AlnGroup.hpp',
    'AlnInfo.hpp',
    'MovieInfo.hpp',
    'RefGroup.hpp',
    'RefInfo.hpp']),
  subdir : 'libblasr/pbdata/saf')
