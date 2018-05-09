###########
# Sources #
###########

libblasr_sources += files([
  'BaseTuple.cpp',
  'DNATuple.cpp',
  'TupleMetrics.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'BaseTuple.hpp',
    'CompressedDNATuple.hpp',
    'CountedTuple.h',
    'DNATuple.hpp',
    'DNATupleImpl.hpp',
    'DNATupleList.h',
    'HashedTupleList.hpp',
    'HashedTupleListImpl.hpp',
    'TupleCountTable.hpp',
    'TupleCountTableImpl.hpp',
    'tuple.h',
    'TupleList.hpp',
    'TupleListImpl.hpp',
    'TupleMask.h',
    'TupleMatching.hpp',
    'TupleMatchingImpl.hpp',
    'TupleMetrics.hpp',
    'TupleOperations.h',
    'TupleTranslations.h']),
  subdir : 'libblasr/alignment/tuples')
