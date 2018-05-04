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

##########
# Config #
##########

libblasr_libconfig_h_config = configuration_data()
libblasr_libconfig_h_config.set('USE_PBBAM', 1)
libblasr_libconfig_h = configure_file(output : 'LibBlasrConfig.h', configuration : libblasr_libconfig_h_config)

libblasr_sources += libblasr_libconfig_h

###########
# Sources #
###########

libblasr_include_directories += include_directories('.')

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
  libblasr_libconfig_h,
  subdir : 'libblasr/pbdata')

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
