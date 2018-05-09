###########
# Sources #
###########

libblasr_sources += files(['TitleTable.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'FindRandomSequence.hpp',
    'SequenceIndexDatabase.hpp',
    'SequenceIndexDatabaseImpl.hpp',
    'TitleTable.hpp']),
  subdir : 'libblasr/pbdata/metagenome')
