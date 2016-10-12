#ifndef HEADER_H
# define HEADER_H

# include <easy_sdl.h>
# include <vec.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct			s_material
{
	Vec3f				surf_color;
	Vec3f				emis_color;
	float				spec_value;
	float				spec_power;
}						t_material;
typedef t_material 		Material;

typedef struct			s_sphere
{
	Vec3f				pos;
	float				rad;
	Material			mat;
	int					is_light;
}						t_sphere;
typedef t_sphere		Sphere;

typedef struct			s_spheres
{
	size_t				nb_spheres;
	Sphere				*spheres;
}						t_spheres;
typedef t_spheres		Spheres;

typedef struct			s_ray
{
	Vec3f				pos;
	Vec3f				dir;
}						t_ray;
typedef t_ray			Ray;

typedef struct			s_data
{
	Spheres				spheres;
	t_esdl				*esdl;
	SDL_Surface			*surf;
	SDL_Texture			*tex;

	Vec3f				plane_pos;
	Vec3f				plane_norm;
}						t_data;

int			init_spheres(size_t nb_spheres, Spheres *spheres);
void		free_spheres(Spheres *spheres);
Sphere		set_sphere(Vec3f pos, float radius, Material mat);
Sphere		set_light(Vec3f pos, float radius, Material mat);
int			sphere_intersect(Ray *ray, Sphere sphere, float *t0, float *t1);

Material	set_material(Vec3f surf_color, Vec3f emis_color, float spec_value, float spec_power);

#endif
