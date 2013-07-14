#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'ckodo'
VERSION = '1.0.0'

def recurse_helper(ctx, name):
    if not ctx.has_dependency_path(name):
        ctx.fatal('Load a tool to find %s as system dependency' % name)
    else:
        p = ctx.dependency_path(name)
        ctx.recurse(p)

def options(opt):

    opt.load('wurf_tools')
    opt.load('wurf_dependency_bundle')

    import waflib.extras.wurf_dependency_bundle as bundle
    import waflib.extras.wurf_dependency_resolve as resolve
    import waflib.extras.wurf_configure_output

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'waf-tools',
            git_repository = 'github.com/steinwurf/external-waf-tools.git',
            major_version = 2))

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'gtest',
            git_repository = 'github.com/steinwurf/external-gtest.git',
            major_version = 2))

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'boost',
            git_repository = 'github.com/steinwurf/external-boost-light.git',
            major_version = 1))

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'sak',
            git_repository = 'github.com/steinwurf/sak.git',
            major_version = 10))

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'fifi',
            git_repository = 'github.com/steinwurf/fifi.git',
            major_version = 9))

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
            name = 'gauge',
            git_repository = 'github.com/steinwurf/cxx-gauge.git',
            major_version = 5))

    bundle.add_dependency(opt,
        resolve.ResolveGitMajorVersion(
             name = 'kodo',
             git_repository = 'github.com/steinwurf/kodo.git',
             major_version = 11))


def configure(conf):
    
    if conf.is_toplevel():

        conf.load('wurf_dependency_bundle')
        conf.load('wurf_tools')

        conf.load_external_tool('mkspec', 'wurf_cxx_mkspec_tool')
        conf.load_external_tool('runners', 'wurf_runner')
        conf.load_external_tool('install_path', 'wurf_install_path')
        conf.load_external_tool('project_gen', 'wurf_project_generator')

        recurse_helper(conf, 'boost')
        recurse_helper(conf, 'gtest')
        recurse_helper(conf, 'sak')
        recurse_helper(conf, 'fifi')
        recurse_helper(conf, 'gauge')
        recurse_helper(conf, 'kodo')

def build(bld):

    if bld.is_toplevel():

        bld.load('wurf_dependency_bundle')

        recurse_helper(bld, 'boost')
        recurse_helper(bld, 'gtest')
        recurse_helper(bld, 'sak')
        recurse_helper(bld, 'fifi')
        recurse_helper(bld, 'gauge')
        recurse_helper(bld, 'kodo')

        bld.stlib(source = 'src/ckodo/ckodo.cpp',
                  target = 'ckodo',
                  name   = 'ckodo_static',
                  export_includes = 'src',
                  use    = ['kodo_includes', 'boost_includes',
                            'fifi_includes', 'sak_includes'])
                            
        bld.shlib(source = 'src/ckodo/ckodo.cpp',
                  target = 'ckodo',
                  name   = 'ckodo_shared',
                  install_path = None,
                  export_includes = 'src',
                  use    = ['kodo_includes', 'boost_includes',
                            'fifi_includes', 'sak_includes'])

        bld.recurse('test')
        bld.recurse('examples/encode_decode_simple')


def dist(ctx):
    excludes = 'build **/.git **/.gitignore **/*~ **/*.pyc .lock* *.bat ' \
               'waf-* .waf-* *.zip bundle_dependencies/*/master/*'
    ctx.base_name = APPNAME+'-standalone-'+VERSION 
    ctx.algo      = 'zip'    
    ctx.files     = ctx.path.ant_glob('**/*', dir = True, excl = excludes.split(' '))
