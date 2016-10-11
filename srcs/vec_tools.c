#include <vec.h>
#include <math.h>

inline Vec3f				set_vec(float x, float y, float z)
{
	Vec3f			ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

inline Vec3f               vec_sub(Vec3f *v1, Vec3f *v2)
{
    Vec3f           ret;

    ret.x = v1->x - v2->x;
    ret.y = v1->y - v2->y;
    ret.z = v1->z - v2->z;
    return (ret);
}

inline Vec3f				vec_add(Vec3f *v1, Vec3f *v2)
{
	Vec3f			ret;

	ret.x = v1->x + v2->x;
	ret.y = v1->y + v2->y;
	ret.z = v1->z + v2->z;
	return (ret);
}

inline Vec3f				vec_add_f(Vec3f *v1, float value)
{
	Vec3f			ret;

	ret.x = v1->x + value;
	ret.y = v1->y + value;
	ret.z = v1->z + value;
	return (ret);
}

inline Vec3f         vec_mult_f(Vec3f *v, float f)
{
    Vec3f     ret;

    ret.x = v->x * f;
    ret.y = v->y * f;
    ret.z = v->z * f;
    return (ret);
}

inline Vec3f				vec_mult(Vec3f *v1, Vec3f *v2)
{
	Vec3f			ret;

	ret.x = v1->x * v2->x;
	ret.y = v1->y * v2->y;
	ret.z = v1->z * v2->z;
	return (ret);
}

inline float				dot_product(Vec3f *v1, Vec3f *v2)
{
	return (v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}

inline float				vec_length(Vec3f *vec)
{
	return (sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z));
}

Vec3f				vec_normalize(Vec3f *vec)
{
	Vec3f			ret;
	float			len;

	len = vec_length(vec);
	ret.x = vec->x / len;
	ret.y = vec->y / len;
	ret.z = vec->z / len;
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

#include <stdio.h>
void				debug_vec(Vec3f vec)
{
	printf("x = %f y = %f z = %f\n", vec.x, vec.y, vec.z);
}