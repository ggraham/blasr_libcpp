project(
  'libblasr',
  'cpp',
  version : '5.3.0',
  default_options : [
    'buildtype=release',
    'warning_level=3',
    'cpp_std=c++14'],
  license : 'BSD-3',
  meson_version : '>= 0.41.1')

############
# CXXFLAGS #
############

libblasr_warning_flags = []
cpp = meson.get_compiler('cpp')
foreach cflag: [
  '-Wno-delete-non-virtual-dtor',
  '-Wno-sign-compare',
  '-Wno-unused-parameter',
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

# HDF5
libhdf5_cflags = []
if get_option('HDF5_CFLAGS') != ''
  libhdf5_cflags = get_option('HDF5_CFLAGS').split(' ')
endif

libhdf5_libs = []
if get_option('HDF5_LIBS') != ''
  libhdf5_libs = get_option('HDF5_LIBS').split(' ')
endif

# boost
libblasr_boost_dep = dependency('boost', required : true)

# pbbam
libblasr_pbbam_dep = dependency('pbbam', fallback : ['pbbam', 'pbbam_dep'])

#####################
# sources + library #
#####################

libblasr_sources = []
libblasr_include_directories = [include_directories('.')]

subdir('hdf')
subdir('alignment')
subdir('pbdata')

libblasr_lib = library(
  'blasr',
  libblasr_sources,
  install : true,
  soversion : 0,
  version : meson.project_version(),
  include_directories : libblasr_include_directories,
  dependencies : [libblasr_pbbam_dep, libblasr_boost_dep],
  cpp_args : [libhdf5_cflags, libblasr_warning_flags, '-DUSE_PBBAM=1'],
  link_args : libhdf5_libs)

#########
# tests #
#########

if get_option('enable-blasr-libcpp-tests')
  subdir('unittest')
endif

###################
# dependency info #
###################

import('pkgconfig').generate(
  libraries : libblasr_lib,
  version : meson.project_version(),
  name : 'libblasr',
  requires : 'pbbam',
  filebase : 'libblasr',
  subdirs : ['libblasr', 'libblasr/alignment', 'libblasr/hdf', 'libblasr/pbdata'],
  description : 'Library routines used by the PacBio BLASR aligner')

libblasr_dep = declare_dependency(
  include_directories : libblasr_include_directories,
  link_with : libblasr_lib,
  dependencies : libblasr_pbbam_dep,
  version : meson.project_version(),
  sources : libblasr_sources,
  compile_args : libhdf5_cflags + libblasr_warning_flags,
  link_args : libhdf5_libs)
