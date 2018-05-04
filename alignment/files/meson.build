###########
# Sources #
###########

libblasr_sources += files([
  'BaseSequenceIO.cpp',
  'CCSIterator.cpp',
  'FragmentCCSIterator.cpp',
  'ReaderAgglomerate.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'BaseSequenceIO.hpp',
    'CCSIterator.hpp',
    'FragmentCCSIterator.hpp',
    'ReaderAgglomerate.hpp',
    'ReaderAgglomerateImpl.hpp']),
  subdir : 'libblasr/alignment/files')
