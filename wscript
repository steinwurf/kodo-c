#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'kodoc'
VERSION = '6.0.0'

import waflib.extras.wurf_options


def options(opt):

    opt.load('wurf_common_tools')


def resolve(ctx):

    import waflib.extras.wurf_dependency_resolve as resolve

    ctx.load('wurf_common_tools')

    ctx.add_dependency(resolve.ResolveVersion(
        name='waf-tools',
        git_repository='github.com/steinwurf/waf-tools.git',
        major=3))

    ctx.add_dependency(resolve.ResolveVersion(
        name='kodo',
        git_repository='github.com/steinwurf/kodo.git',
        major=33))

    # Internal dependencies
    if ctx.is_toplevel():

        ctx.add_dependency(resolve.ResolveVersion(
            name='gtest',
            git_repository='github.com/steinwurf/gtest.git',
            major=3))


def configure(conf):

    conf.load("wurf_common_tools")


def build(bld):

    CXX = bld.env.get_flat("CXX")
    # Matches both g++ and clang++
    if 'g++' in CXX or 'clang' in CXX:
        # The -fPIC is required for all underlying static libraries that
        # will be included in the shared library
        bld.env.append_value('CXXFLAGS', '-fPIC')
        # Hide most of the private symbols in the shared library to decrease
        # its size and improve its load time
        bld.env.append_value('CXXFLAGS', '-fvisibility=hidden')
        bld.env.append_value('CXXFLAGS', '-fvisibility-inlines-hidden')
        bld.env.append_value('LINKFLAGS', '-fvisibility=hidden')

    bld.load("wurf_common_tools")

    bld.env.append_unique(
        'DEFINES_STEINWURF_VERSION',
        'STEINWURF_KODOC_VERSION="{}"'.format(VERSION))

    bld.stlib(
        source=bld.path.ant_glob('src/kodoc/*.cpp'),
        target='kodoc_static',
        name='kodoc_static',
        defines=['KODOC_STATIC'],
        export_defines=['KODOC_STATIC'],
        export_includes='src',
        use=['kodo_includes'])

    # Define the task generator that will build the kodoc shared library
    gen = bld.shlib(
        source=bld.path.ant_glob('src/kodoc/*.cpp'),
        target='kodoc',
        name='kodoc',
        defines=['KODOC_DLL_EXPORTS'],
        install_path=None,
        export_includes='src',
        use=['kodo_includes'])

    # Make sure that the task generator is posted, which is necessary in
    # order to access the task generator by name in child projects.
    # We need this to get the location of the compiled shared library
    # when running the unit tests
    gen.post()

    # Define the applications after the 'kodoc' task generator is posted
    if bld.is_toplevel():

        bld.recurse('test')
        bld.recurse('examples/encode_decode_on_the_fly')
        bld.recurse('examples/encode_decode_simple')
        bld.recurse('examples/raw_symbols')
        bld.recurse('examples/shallow_encode_decode')
        bld.recurse('examples/sliding_window')
        bld.recurse('examples/switch_systematic_on_off')
        bld.recurse('examples/udp_sender_receiver')
        bld.recurse('examples/use_trace_layers')
        bld.recurse('benchmark/kodoc_throughput')
