project('rtoneweekend', 'cpp')

incdir = include_directories('include')
srcdir = include_directories('include')

#subdir('src')
#subdir('include')

sources = ['src/main.cpp']
includes = [
	'src/camera.h',
	'src/dialectric.h',
	'src/hitable.h',
	'src/HitableList.h',
	'src/lambertian.h',
	'src/metal.h',
	'src/ray.h',
	'src/sphere.h',
	'src/vec3.h'
]

rt_exec = executable('rt_exec', [sources, includes])
