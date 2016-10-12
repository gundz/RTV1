#include <rtv1.h>

int			init_objects(size_t nb_obj, Objects *objects)
{
	objects->nb_obj = nb_obj;
	objects->objects = NULL;
	if (!(objects->objects = (Object *)malloc(sizeof(Object) * nb_obj)))
		return (0);
	return (1);
}

void		free_objects(Objects *objects)
{
	free(objects->objects);
}

Object		set_sphere(Vec3f pos, float radius, Material mat)
{
	Object	object;

	object.objtype = SPHERE;
	object.pos = pos;
	object.rad = radius;
	object.mat = mat;
	object.is_light = 0;
	return (object);
}

Object		set_light(Vec3f pos, float radius, Material mat)
{
	Object	light;

	light.pos = pos;
	light.rad = radius;
	light.mat = mat;
	light.is_light = 1;
	return (light);
}

int			sphere_intersect(Ray *ray, Object object, float *t0, float *t1)
{
	t_vec l = vec_sub(object.pos, ray->pos);
	float tca = dot_product(l, ray->dir);
	if (tca < 0)
	    return (0);
	float d2 = dot_product(l, l) - tca * tca;
	if (d2 > (object.rad * object.rad))
	    return (0);
	float thc = sqrtf((object.rad * object.rad) - d2);
	*t0 = tca - thc;
	*t1 = tca + thc;
	return (1);
}