#ifndef VEC_H
# define VEC_H

typedef struct			s_vec
{
	float				x;
	float				y;
	float				z;
}						t_vec;
typedef t_vec			Vec3f;


Vec3f				set_vec(float x, float y, float z);

Vec3f               vec_sub(Vec3f v1, Vec3f v2);

Vec3f               vec_sub_f(t_vec v1, float f);

Vec3f				vec_add(Vec3f v1, Vec3f v2);

Vec3f				vec_add_f(Vec3f v1, float value);

Vec3f         		vec_mult_f(Vec3f v, float f);

Vec3f				vec_mult(Vec3f v1, Vec3f v2);

Vec3f				cross_product(Vec3f a,Vec3f b);

float				dot_product(Vec3f v1, Vec3f v2);

float				vec_length(Vec3f vec);

Vec3f				vec_normalize(Vec3f vec);

float				max(float a, float b);

float				min(float a, float b);

void				debug_vec(Vec3f vec);

#endif