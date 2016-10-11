#include <rtv1.h>

Material			set_material(Vec3f surf_color, Vec3f emis_color, float spec_value, float spec_power)
{
	Material		mat;

	mat.surf_color = surf_color;
	mat.emis_color = emis_color;
	mat.spec_value = spec_value;
	mat.spec_power = spec_power;
	return (mat);
}