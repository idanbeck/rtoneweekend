project('rtoneweekend', 'cpp')

#incdir = include_directories('include')
#srcdir = include_directories('src')

#subdir('src')
#subdir('include')

sources = ['src/main.cpp']
includes = [
	'src/camera.h',
	'src/material.h',
	'src/dialectric.h',
	'src/hitable.h',
	'src/HitableList.h',
	'src/lambertian.h',
	'src/metal.h',
	'src/ray.h',
	'src/sphere.h',
	'src/vec3.h',
	'src/aabb.h',
	'src/BVHNode.h',
	'src/texture.h',
	'src/ConstantTexture.h',
	'src/CheckerTexture.h',
	'src/NoiseTexture.h',
	'src/ConstantMedium.h',
	'src/perlin.h',
	'src/drand48.h',
	'src/DiffuseLight.h',
	'src/box.h',
	'src/rect.h'
]

rt_exec = executable('rt_exec', [sources, includes])
