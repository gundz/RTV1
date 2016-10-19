#ifndef HEADER_H
# define HEADER_H

# include <easy_sdl.h>
# include <vec.h>
# include <unistd.h>
# include <sys/time.h>

typedef enum			e_object_type
{
	SPHERE, PLANE
}						t_object_type;
typedef t_object_type	ObjType;

typedef struct			s_material
{
	Vec3f				surf_color;
	Vec3f				emis_color;
	float				spec_value;
	float				spec_power;
}						t_material;
typedef t_material 		Material;

typedef struct			s_object
{
	Vec3f				pos;
	float				rad;
	Vec3f				norm;
	Material			mat;
	int					is_light;
	int					type;
}						t_object;
typedef t_object		Object;

typedef struct			s_objects
{
	size_t				nb_obj;
	Object				*objects;
}						t_objects;
typedef t_objects		Objects;

typedef struct			s_ray
{
	Vec3f				pos;
	Vec3f				dir;
}						t_ray;
typedef t_ray			Ray;

typedef struct			s_data
{
	Objects				objects;
	t_esdl				*esdl;
	SDL_Surface			*surf;
	SDL_Texture			*tex;

	Vec3f				plane_pos;
	Vec3f				plane_norm;
}						t_data;

int			init_objects(size_t nb_spheres, Objects *objects);
void		free_objects(Objects *objects);

Object		set_sphere(Vec3f pos, float radius, Material mat);
Object		set_light(Vec3f pos, float radius, Material mat);
int			sphere_inter(Ray *ray, Object object, float *t0, float *t1);

int			plane_inter(Ray ray, Vec3f pos, Vec3f normal, float *t);
Object		set_plane(Vec3f pos, Vec3f norm, Material mat);

Material	set_material(Vec3f surf_color, Vec3f emis_color, float spec_value, float spec_power);

float		calc_lambert(Vec3f lightDirection, Vec3f nhit);
float		calc_phong(Vec3f light_dir, Ray hit, Ray ray, Material mat);

#endif
