###########
# Sources #
###########

libblasr_sources += files([
  'MetricField.cpp',
  'MovieAlnIndexLookupTable.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'MetricField.hpp',
    'MovieAlnIndexLookupTable.hpp']),
  subdir : 'libblasr/pbdata/loadpulses')
