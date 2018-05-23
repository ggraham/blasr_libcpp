##################
# Subdirectories #
##################

subdir('alignment')
subdir('amos')
subdir('loadpulses')
subdir('matrix')
subdir('metagenome')
subdir('qvs')
subdir('reads')
subdir('saf')
subdir('sam')
subdir('utils')

###########
# Sources #
###########

libblasr_sources += files([
  'BlasrFASTAReader.cpp',
  'BlasrFASTQReader.cpp',
  'CCSSequence.cpp',
  'ChangeListID.cpp',
  'CommandLineParser.cpp',
  'DNASequence.cpp',
  'FASTASequence.cpp',
  'FASTQSequence.cpp',
  'GFFFile.cpp',
  'MD5Utils.cpp',
  'NucConversion.cpp',
  'PackedDNASequence.cpp',
  'ReverseCompressIndex.cpp',
  'SMRTSequence.cpp',
  'StringUtils.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'CCSSequence.hpp',
    'ChangeListID.hpp',
    'CommandLineParser.hpp',
    'Compare4BitCompressed.hpp',
    'CompressedDNASequence.hpp',
    'CompressedSequence.hpp',
    'CompressedSequenceImpl.hpp',
    'defs.h',
    'DNASequence.hpp',
    'Enumerations.h',
    'FASTAReader.hpp',
    'FASTASequence.hpp',
    'FASTQReader.hpp',
    'FASTQSequence.hpp',
    'GFFFile.hpp',
    'MD5Utils.hpp',
    'MD5UtilsImpl.hpp',
    'NucConversion.hpp',
    'PacBioDefs.h',
    'PackedDNASequence.hpp',
    'PrettyException.hpp',
    'ReverseCompressIndex.hpp',
    'SeqUtils.hpp',
    'SeqUtilsImpl.hpp',
    'SMRTSequence.hpp',
    'StringUtils.hpp',
    'Types.h',
    'utils.hpp',
    'utilsImpl.hpp',
    'VectorUtils.hpp']),
  subdir : 'libblasr/pbdata')
