###########
# Sources #
###########

libblasr_sources += files([
  'SAMQVConversion.cpp',
  'SAMSupplementalQVList.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'AlignmentSetToCmpH5Adapter.hpp',
    'AlignmentSetToCmpH5AdapterImpl.hpp',
    'SAMQVConversion.hpp',
    'SAMSupplementalQVList.hpp']),
  subdir : 'libblasr/alignment/datastructures/alignmentset')
