#include <rtv1.h>

int			init_spheres(size_t nb_spheres, Spheres *spheres)
{
	spheres->nb_spheres = nb_spheres;
	spheres->spheres = NULL;
	if (!(spheres->spheres = (Sphere *)malloc(sizeof(Sphere) * nb_spheres)))
		return (0);
	return (1);
}

void		free_spheres(Spheres *spheres)
{
	free(spheres->spheres);
}

Sphere		set_sphere(Vec3f pos, float radius, Material mat)
{
	Sphere	sphere;

	sphere.pos = pos;
	sphere.rad = radius;
	sphere.mat = mat;
	sphere.is_light = 0;
	return (sphere);
}

int			sphere_intersect(t_vec rayorig, t_vec raydir, Sphere sphere, float *t0, float *t1)
{
	t_vec l = vec_sub(&sphere.pos, &rayorig);
	float tca = dot_product(&l, &raydir);
	if (tca < 0)
	    return (0);
	float d2 = dot_product(&l, &l) - tca * tca;
	if (d2 > (sphere.rad * sphere.rad))
	    return (0);
	float thc = sqrtf((sphere.rad * sphere.rad) - d2);
	*t0 = tca - thc;
	*t1 = tca + thc;
	return (1);
}