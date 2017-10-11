##################
# Subdirectories #
##################

libblasr_unittest_sources = []
subdir('alignment')
subdir('hdf')
subdir('pbdata')

###########
# Sources #
###########

libblasr_gtest_dep = dependency('gtest', fallback : ['gtest', 'gtest_with_main_dep'])

libblasr_unittest = executable(
  'libblasr_unittest',
  libblasr_unittest_sources,
  dependencies : [
     libblasr_pbbam_dep,
     libblasr_gtest_dep],
  include_directories : libblasr_include_directories,
  link_with : libblasr_lib,
  cpp_args : [libhdf5_cflags, libblasr_warning_flags, '-DUSE_PBBAM=1'],
  link_args : libhdf5_libs,
  install : false)

#########
# tests #
#########

test(
  'check_libcpp',
  libblasr_unittest,
  args : [
    '--gtest_output=xml:' + meson.current_build_dir() + '/libcpp-unit.xml'],
  workdir : meson.source_root(),
  timeout : 1800)
