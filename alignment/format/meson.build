###########
# Sources #
###########

libblasr_sources += files([
  'CompareSequencesPrinter.cpp',
  'IntervalPrinter.cpp',
  'SAMHeaderPrinter.cpp',
  'SAMPrinter.cpp',
  'SummaryPrinter.cpp'])

###########
# Headers #
###########

meson.is_subproject() and subdir_done()

install_headers(
  files([
    'BAMPrinter.hpp',
    'BAMPrinterImpl.hpp',
    'CompareSequencesPrinter.hpp',
    'CompareSequencesPrinterImpl.hpp',
    'IntervalPrinter.hpp',
    'SAMHeaderPrinter.hpp',
    'SAMPrinter.hpp',
    'SAMPrinterImpl.hpp',
    'StickAlignmentPrinter.hpp',
    'SummaryPrinter.hpp',
    'VulgarPrinter.hpp',
    'XMLPrinter.hpp']),
  subdir : 'libblasr/alignment/format')
