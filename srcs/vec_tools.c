#include <rtv1.h>

t_vec				set_vec(float x, float y, float z)
{
	t_vec			ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

t_vec               vec_sub(t_vec v1, t_vec v2)
{
    t_vec           ret;

    ret.x = v1.x - v2.x;
    ret.y = v1.y - v2.y;
    ret.z = v1.z - v2.z;
    return (ret);
}

t_vec               vec_sub_f(t_vec v1, float f)
{
    t_vec           ret;

    ret.x = v1.x - f;
    ret.y = v1.y - f;
    ret.z = v1.z - f;
    return (ret);
}

t_vec				vec_add(t_vec v1, t_vec v2)
{
	t_vec			ret;

	ret.x = v1.x + v2.x;
	ret.y = v1.y + v2.y;
	ret.z = v1.z + v2.z;
	return (ret);
}

t_vec				vec_add_f(t_vec v1, float value)
{
	t_vec			ret;

	ret.x = v1.x + value;
	ret.y = v1.y + value;
	ret.z = v1.z + value;
	return (ret);
}

t_vec         vec_mult_f(t_vec v, float f)
{
    t_vec     ret;

    ret.x = v.x * f;
    ret.y = v.y * f;
    ret.z = v.z * f;
    return (ret);
}

t_vec				vec_mult(t_vec v1, t_vec v2)
{
	t_vec			ret;

	ret.x = v1.x * v2.x;
	ret.y = v1.y * v2.y;
	ret.z = v1.z * v2.z;
	return (ret);
}

Vec3f				cross_product(Vec3f a,Vec3f b)
{
	return (set_vec(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x));
}

float				dot_product(t_vec v1, t_vec v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float				vec_length(t_vec vec)
{
	return (sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

t_vec				vec_normalize(t_vec vec)
{
	t_vec			ret;
	float			len;

	len = vec_length(vec);
	ret.x = vec.x / len;
	ret.y = vec.y / len;
	ret.z = vec.z / len;
	return (ret);
}

float				max(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

float				min(float a, float b)
{
	if (a < b)
		return (a);
	return (b);
}