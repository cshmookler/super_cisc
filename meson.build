# Declare project information
project(
    'super_cisc',
    'cpp',
    version : files('VERSION'),
    default_options : [ 'cpp_std=c++17' ],
)

# Project root and build directories
root_dir = meson.project_source_root()
build_dir = meson.project_build_root()
src_dir = root_dir / 'src'
tests_dir = root_dir / 'tests'

# Insert the project version into the version header file
conf_data = configuration_data()
conf_data.set('version', meson.project_version())
configure_file(
    configuration : conf_data,
    input : src_dir / 'version.hpp.in',
    output : 'version.hpp',
)

exe_snake = executable(
    'snake',
    files(
        src_dir / 'snake.cpp',
        src_dir / 'version.cpp',
    ),
    install : true,
)

dep_gtest_main = dependency(
    'gtest_main',
    required : false,
    method : 'auto',
)

if dep_gtest_main.found()
    test_version = executable(
        'version',
        files(
            tests_dir / 'version.test.cpp',
            src_dir / 'version.cpp',
        ),
        dependencies : dep_gtest_main,
    )
    test('version', test_version)
else
    warning('Skipping tests due to missing dependencies')
endif
