project(
    'bagman',
    'cpp',
    version : '1.2',
    default_options : [
        'cpp_std=c++17',
        'warning_level=3',
        'buildtype=debug'
    ]
)

add_project_arguments(
    '-Werror',
    '-Wno-unused-label',
    '-fmax-errors=50',
    '-DHAS_SDL_IMAGE',
    language : 'cpp'
)

sdl = dependency('sdl')
sdl_mixer = dependency('SDL_mixer')

subdir('src')
