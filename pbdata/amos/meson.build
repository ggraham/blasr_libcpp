###########
# Sources #
###########

libblasr_sources += files(['AfgBasWriter.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files(['AfgBasWriter.hpp']),
  subdir : 'libblasr/pbdata/amos')
