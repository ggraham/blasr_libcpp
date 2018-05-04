###########
# Sources #
###########

libblasr_sources += files([
  'PbiFilterZmwGroupQuery.cpp',
  'SequentialZmwGroupQuery.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'PbiFilterZmwGroupQuery.h',
    'SequentialZmwGroupQuery.h']),
  subdir : 'libblasr/alignment/query')
