#include <rtv1.h>

float			calc_lambert(Vec3f lightDirection, Vec3f nhit)
{
	return (max(0.0f, dot_product(lightDirection, nhit)));
}

float			calc_phong(Vec3f light_dir, Ray hit, Ray ray, Material mat)
{
	Vec3f		view_dir;
	Vec3f		blinn_dir;
	float		blinn_term;

	view_dir = vec_sub(hit.pos, ray.pos);
	view_dir = vec_normalize(view_dir);
	blinn_dir = vec_sub(light_dir, view_dir);
	blinn_dir = vec_normalize(blinn_dir);
	blinn_term = max(dot_product(blinn_dir, hit.dir), 0.0f);
	return (mat.spec_value * powf(blinn_term, mat.spec_power));
}