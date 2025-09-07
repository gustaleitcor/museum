project('opengl-app', 'cpp', version: '0.1', default_options: ['cpp_std=c++23'])

# OpenGL dependencies
opengl = dependency('gl')
glu = dependency('glu')
glut = dependency('glut')

subdir('src')

executable(
  'main',
  sources: src_files,
  include_directories: ['include/models', 'include/utils'],
  dependencies: [opengl, glu, glut],
  install: false,
)
