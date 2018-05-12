##################
# Subdirectories #
##################

subdir('algorithms')
subdir('bwt')
subdir('datastructures')
subdir('files')
subdir('format')
subdir('ipc')
subdir('query')
subdir('qvs')
subdir('simulator')
subdir('statistics')
subdir('suffixarray')
subdir('tuples')
subdir('utils')

###########
# Sources #
###########

libblasr_sources += files(['MappingMetrics.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files(['MappingMetrics.hpp']),
  subdir : 'libblasr/alignment')
