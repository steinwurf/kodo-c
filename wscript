#! /usr/bin/env python
# encoding: utf-8

import os

APPNAME = 'kodoc'
VERSION = '11.0.0'

codecs = ['full_vector', 'on_the_fly', 'sliding_window',
          'sparse_full_vector', 'seed', 'sparse_seed',
          'perpetual', 'fulcrum', 'reed_solomon']


def options(opt):

    opts = opt.add_option_group('kodo-c options')

    opts.add_option(
        '--disable_rlnc', default=None, dest='disable_rlnc',
        action='store_true', help="Disable the basic RLNC codecs")

    opts.add_option(
        '--disable_fulcrum', default=None, dest='disable_fulcrum',
        action='store_true', help="Disable the Fulcrum RLNC codec")

    opts.add_option(
        '--disable_reed_solomon', default=None, dest='disable_reed_solomon',
        action='store_true', help="Disable the Reed-Solomon codec")

    opts.add_option(
        '--enable_codecs', default=None, dest='enable_codecs',
        help="Enable the chosen codec or codecs, and disable all others. "
             "A comma-separated list of these values: {0}".format(codecs))


def configure(conf):

    conf.env['DEFINES_KODOC_COMMON'] = []

    disabled_codec_groups = 0

    if conf.has_tool_option('disable_rlnc') or \
       not conf.has_dependency_path('kodo-rlnc'):
        conf.env['DEFINES_KODOC_COMMON'] += ['KODOC_DISABLE_RLNC']
        disabled_codec_groups += 1
    if conf.has_tool_option('disable_fulcrum') or \
       not conf.has_dependency_path('kodo-fulcrum'):
        conf.env['DEFINES_KODOC_COMMON'] += ['KODOC_DISABLE_FULCRUM']
        disabled_codec_groups += 1
    if conf.has_tool_option('disable_reed_solomon') or \
       not conf.has_dependency_path('kodo-reed-solomon'):
        conf.env['DEFINES_KODOC_COMMON'] += ['KODOC_DISABLE_REED_SOLOMON']
        disabled_codec_groups += 1

    if disabled_codec_groups == 3:
        conf.fatal('All codec groups are disabled or unavailable. Please make '
                   'sure that you enable at least one codec group and you '
                   'have access to the corresponding repositories!')

    if conf.has_tool_option('enable_codecs'):
        enabled = conf.get_tool_option('enable_codecs').split(',')

        # Validate the chosen codecs
        for codec in enabled:
            if codec not in codecs:
                conf.fatal('Invalid codec: "{0}". Please use the following '
                           'codec names: {1}'.format(codec, codecs))

        # Disable the codecs that were not selected
        for codec in codecs:
            if codec not in enabled:
                conf.env['DEFINES_KODOC_COMMON'] += \
                    ['KODOC_DISABLE_{0}'.format(codec.upper())]


def build(bld):

    CXX = bld.env.get_flat("CXX")
    # Matches both g++ and clang++
    if 'g++' in CXX or 'clang' in CXX:
        # The -fPIC flag is required for all underlying static libraries that
        # will be included in the shared library
        bld.env.append_value('CXXFLAGS', '-fPIC')
        # Hide most of the private symbols in the shared library to decrease
        # its size and improve its load time
        bld.env.append_value('CXXFLAGS', '-fvisibility=hidden')
        bld.env.append_value('CXXFLAGS', '-fvisibility-inlines-hidden')
        bld.env.append_value('LINKFLAGS', '-fvisibility=hidden')

    bld.env.append_unique(
        'DEFINES_STEINWURF_VERSION',
        'STEINWURF_KODOC_VERSION="{}"'.format(VERSION))

    use_flags = ['kodo_core_includes', 'KODOC_COMMON']

    if 'KODOC_DISABLE_REED_SOLOMON' not in bld.env['DEFINES_KODOC_COMMON']:
        use_flags += ['kodo_reed_solomon_includes']
    if 'KODOC_DISABLE_RLNC' not in bld.env['DEFINES_KODOC_COMMON']:
        use_flags += ['kodo_rlnc_includes']
    if 'KODOC_DISABLE_FULCRUM' not in bld.env['DEFINES_KODOC_COMMON']:
        use_flags += ['kodo_fulcrum_includes']

    # Build the kodo-c static library
    bld.stlib(
        source=bld.path.ant_glob('src/kodoc/*.cpp'),
        target='kodoc_static',
        name='kodoc_static',
        defines=['KODOC_STATIC'],
        export_defines=['KODOC_STATIC'],
        export_includes='src',
        use=use_flags)

    # Build the kodo-c shared library
    bld.shlib(
        source=bld.path.ant_glob('src/kodoc/*.cpp'),
        target='kodoc',
        name='kodoc',
        defines=['KODOC_DLL_EXPORTS'],
        install_path=None,
        export_includes='src',
        use=use_flags)

    if bld.is_toplevel():

        bld.recurse('test')
        bld.recurse('examples/encode_decode_on_the_fly')
        bld.recurse('examples/encode_decode_simple')
        bld.recurse('examples/fulcrum')
        bld.recurse('examples/perpetual')
        bld.recurse('examples/reed_solomon')
        bld.recurse('examples/sliding_window')
        bld.recurse('examples/sparse_seed')
        bld.recurse('examples/switch_systematic_on_off')
        bld.recurse('examples/symbol_status_updater')
        bld.recurse('examples/udp_sender_receiver')
        bld.recurse('examples/uncoded_symbols')
        bld.recurse('examples/use_trace_layers')
        bld.recurse('benchmark/kodoc_throughput')

        # Install kodoc.h to the 'include' folder
        if bld.has_tool_option('install_path'):
            install_path = bld.get_tool_option('install_path')
            install_path = os.path.abspath(os.path.expanduser(install_path))
            start_dir = bld.path.find_dir('src')
            bld.install_files(os.path.join(install_path, 'include'),
                              ['kodoc/kodoc.h'],
                              cwd=start_dir, relative_trick=True)
