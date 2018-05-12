project(
  'libblasr',
  'cpp',
  version : '5.3.0',
  default_options : [
    'buildtype=release',
    'warning_level=3',
    'cpp_std=c++11',
    'b_ndebug=false'],
  license : 'BSD-3',
  meson_version : '>= 0.46.0')

# TODO:
# try to reenable 'b_ndebug=true'
# this will require replacing a number of asserts
# with proper throw statements

############
# CXXFLAGS #
############

libblasr_warning_flags = []
cpp = meson.get_compiler('cpp')
foreach cflag: [
  '-Wno-delete-non-virtual-dtor',
  '-Wno-unused-variable',
  '-Wno-non-virtual-dtor']
    if cpp.has_argument(cflag)
      libblasr_warning_flags += cflag
    endif
endforeach

if cpp.get_id() == 'clang'
  foreach cflag: [
    '-Wno-unused-local-typedef',
    '-Wno-overloaded-virtual',
    '-Wno-keyword-macro']
      if cpp.has_argument(cflag)
        libblasr_warning_flags += cflag
      endif
  endforeach
endif

################
# dependencies #
################

# boost
libblasr_boost_dep = dependency('boost', required : true)

# pbbam
libblasr_pbbam_dep = dependency('pbbam', fallback : ['pbbam', 'pbbam_dep'])

# zlib
libblasr_zlib_dep = dependency('zlib', required : true)

# clock_gettime on old glibc systems
libblasr_rt_dep = cpp.find_library('rt', required : false)

libblasr_deps = [libblasr_boost_dep, libblasr_pbbam_dep, libblasr_zlib_dep, libblasr_rt_dep]

##########
# Config #
##########

libblasr_libconfig_h_config = configuration_data()
libblasr_libconfig_h_config.set('USE_PBBAM', 1)
libblasr_libconfig_h = configure_file(output : 'LibBlasrConfig.h', configuration : libblasr_libconfig_h_config)

if not meson.is_subproject()
  install_headers(
    libblasr_libconfig_h,
    subdir : 'libblasr')
endif

#####################
# sources + library #
#####################

# install library if
# - either running as a proper project
# - or using shared libraries
libblasr_lib_install = (not meson.is_subproject()) or (get_option('default_library') == 'shared')

libblasr_sources = [libblasr_libconfig_h]
libblasr_include_directories = include_directories('.')

subdir('hdf')
subdir('alignment')
subdir('pbdata')

libblasr_lib = library(
  'blasr', [
    libblasr_libconfig_h,
    libblasr_sources],
  install : libblasr_lib_install,
  # use boost SONAME practice:
  #   cause ld startup issues before
  #   you even have the chance of running
  #   into ABI issues.
  soversion : meson.project_version(),
  version : meson.project_version(),
  include_directories : libblasr_include_directories,
  dependencies : libblasr_deps,
  cpp_args : libblasr_warning_flags)

#########
# tests #
#########

if (not meson.is_subproject()) and get_option('tests')
  libblasr_clang_formatter = find_program('tools/check-formatting')
  subdir('unittest')
endif

###################
# dependency info #
###################

if not meson.is_subproject()
  import('pkgconfig').generate(
    libraries : libblasr_lib,
    version : meson.project_version(),
    name : 'libblasr',
    requires : 'pbbam',
    filebase : 'libblasr',
    subdirs : 'libblasr',
    description : 'Library routines used by the PacBio BLASR aligner')
endif

libblasr_dep = declare_dependency(
  include_directories : libblasr_include_directories,
  link_with : libblasr_lib,
  dependencies : libblasr_deps,
  version : meson.project_version(),
  sources : libblasr_libconfig_h)
