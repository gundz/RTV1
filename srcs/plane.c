#include <rtv1.h>

int					plane_inter(Ray ray, Vec3f pos, Vec3f normal, float *t)
{
	float			denom;

	ray.dir = vec_normalize(ray.dir);
	denom = dot_product(normal, ray.dir);
	if (fabs(denom) > 1e-6)
	{
		*t = dot_product(vec_sub(pos, ray.pos), normal) / denom;
		if (*t >= 0)
			return (1);
	}
	return (0);
}

Object				set_plane(Vec3f pos, Vec3f norm, Material mat)
{
	Object			object;

	object.type = PLANE;
	object.pos = pos;
	object.norm = norm;
	object.mat = mat;
	return (object);
}