/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgundlac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/16 15:36:17 by fgundlac          #+#    #+#             */
/*   Updated: 2016/09/16 15:36:17 by fgundlac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <easy_sdl.h>
#include <rtv1.h>

Object				set_cylinder(Vec3f a, Vec3f b, int rad, Material mat)
{
	Object			cylinder;

	cylinder.type = CYLINDER;
	cylinder.pos = a;
	cylinder.norm = b;
	cylinder.rad = rad;
	cylinder.mat = mat;
	return (cylinder);
}

int					cylinder_inter(Ray *ray, Object *object, float *t0, float *t1)
{
	Vec3f			A = object->pos;
	Vec3f			B = object->norm;
	Vec3f			V = ray->dir;

	Vec3f			AB = vec_sub(B, A);
	Vec3f			AO = vec_sub(ray->pos, A);
	Vec3f			AOxAB = cross_product(AO, AB);
	Vec3f			VxAB = cross_product(V, AB);
	float 			ab2 = dot_product(AB, AB);
	float			a = dot_product(VxAB, VxAB);
	float			b = 2 * dot_product(VxAB, AOxAB);
	float			c = dot_product(AOxAB, AOxAB) - ((object->rad * object->rad) * ab2);

	float delta = (b * b) - (4 * a * c);
	if (delta < 0.0f)
		return (0);
	if (delta == 0.0f)
		return (1);
	*t0 = (-b - sqrt(delta) / (2 * a));
	*t1 = (-b + sqrt(delta) / (2 * a));
	return (1);
}

Object				*trace_objects(Ray *ray, Objects *objects, float *tnear)
{
	float			t0;
	float			t1;
	Object			obj;
	Object			*current_obj;

	current_obj = NULL;
	*tnear = INFINITY;
	for (size_t i = 0; i < objects->nb_obj; i++)
	{
		obj = objects->objects[i];
		t0 = INFINITY;
		t1 = INFINITY;
		if (obj.type == CYLINDER && cylinder_inter(ray, &obj, &t0, &t1))
		{
			if (t0 < 0)
				t0 = t1;
			if (t0 < *tnear)
			{
				*tnear = t0;
				current_obj = &(objects->objects[i]);
			}
		}
		if (obj.type == SPHERE && sphere_inter(ray, obj, &t0, &t1))
		{
			if (t0 < 0)
				t0 = t1;
			if (t0 < *tnear)
			{
				*tnear = t0;
				current_obj = &(objects->objects[i]);
			}
		}
		if (obj.type == PLANE && plane_inter(*ray, obj.pos, obj.norm, &t0))
		{
			if (t0 < *tnear)
			{
				*tnear = t0;
				current_obj = &(objects->objects[i]);
			}
		}
	}
	return (current_obj);
}

int					is_shadowed(Ray hit, Vec3f light_dir, Objects *objects, size_t i)
{
	float			t0;
	Object			obj;
	Ray				ray;

	for (size_t j = 0; j < objects->nb_obj; j++)
	{
		if (i == j)
			continue ;
		obj = objects->objects[j];
		ray.pos = vec_add(hit.pos, vec_mult_f(hit.dir, 1e-4));
		ray.dir = light_dir;
		if (obj.type == SPHERE && sphere_inter(&ray, obj, &t0, &t0) == 1)
			return (0);
		ray.pos = vec_add(hit.pos, obj.norm);
		if (obj.type == PLANE && plane_inter(ray, obj.pos, obj.norm, &t0) == 1)
			return (0);
	}
	return (1);
}

Vec3f				lighting(Ray *ray, Ray hit, Objects *objects, Object *current_obj)
{
	Vec3f			color;
	Object			current_light;
	Vec3f			light_dir;
	float			lambert = 0.0f;
	float			phong = 0.0f;

	color = set_vec(0.0f, 0.0f, 0.0f);
	for (size_t i = 0; i < objects->nb_obj; i++)
	{
		if (objects->objects[i].is_light == 1)
		{
			current_light = objects->objects[i];
			light_dir = vec_sub(current_light.pos, hit.pos);
			light_dir = vec_normalize(light_dir);
			lambert = 0.0f;
			phong = 0.0f;
			if (is_shadowed(hit, light_dir, objects, i))
			{
				lambert = calc_lambert(light_dir, hit.dir);
				phong = calc_phong(light_dir, hit, *ray, current_obj->mat);
			}
			color = vec_add(color, vec_mult(current_obj->mat.surf_color, vec_add( vec_mult_f(current_light.mat.emis_color, lambert), vec_mult_f(current_obj->mat.surf_color, phong))));
		}
	}
	return (vec_mult(current_obj->mat.surf_color, vec_add(color, current_obj->mat.emis_color)));
}

Vec3f				raytrace(Ray *ray, t_data *data)
{
	float			tnear = INFINITY;
	Object			*current_obj = NULL;

	//current_obj = trace_objects(ray, &(data->objects), &tnear);

	for (size_t i = 0; i < data->objects.nb_obj; i++)
	{
		Object obj = data->objects.objects[i];
		float t0 = INFINITY;
		float t1 = INFINITY;
		if (obj.type == CYLINDER && cylinder_inter(ray, &obj, &t0, &t1))
		{
			if (t0 < 0)
				t0 = t1;
			if (t0 < tnear)
			{
				tnear = t0;
				current_obj = &(data->objects.objects[i]);
			}
			//return (set_vec(1.0f, 1.0f, 1.0f));
		}
	}

	if (current_obj == NULL)
		return (set_vec(0.0f, 0.0f, 0.0f));

	Ray hit;
	hit.pos = vec_add(ray->pos, vec_mult_f(ray->dir, tnear));
	hit.dir = vec_sub(hit.pos, current_obj->pos);
	hit.dir = vec_normalize(hit.dir);

	Vec3f color = lighting(ray, hit, &(data->objects), current_obj);
	return (color);
}

Vec3f			pixelCoordinateToWorldCoordinate(float x, float y, int rx, int ry)
{
	Vec3f		ret;

	float invWidth = 1.0f / (float)rx;
	float invHeight = 1.0f / (float)ry;
	float fov = 30.0f;
	float aspectratio = rx / (float)ry;
	float angle = tan(M_PI * 0.5f * fov / 180.0f);

    ret.x = (2.0f * ((x + 0.5f) * invWidth) - 1.0f) * angle * aspectratio;
    ret.y = (1.0f - 2.0f * ((y + 0.5f) * invHeight)) * angle;
    ret.z = -1.0f;
    return (ret);
}

void				render(t_data *data)
{
	Ray				ray;

	t_vec camera_pos = set_vec(0.0f, 5.0f, 10.0f);
	Vec3f cameraLookAt = set_vec(0.0f, -0.2f, 0.0f);

	for (int y = 0; y < SDL_RY; y++)
	{
		for (int x = 0; x < SDL_RX; x++)
		{
			ray.dir = pixelCoordinateToWorldCoordinate(x, y, SDL_RX, SDL_RY);

			ray.dir = vec_add(ray.dir, cameraLookAt);
			ray.dir = vec_normalize(ray.dir);

			ray.pos = camera_pos;
			Vec3f vec_color = raytrace(&ray, data);


			int red = min(1.0f, vec_color.x) * 255;
			int green = min(1.0f, vec_color.y) * 255;
			int blue = min(1.0f, vec_color.z) * 255;
			int color = red << 24 | green << 16 | blue << 8 | 255;
            esdl_put_pixel(data->surf, x, y, color);
		}
	}
}

void				init(t_data *data)
{
	data->surf = esdl_create_surface(SDL_RX, SDL_RY);

	Material		white = set_material(set_vec(0.9f, 0.9f, 0.9f), set_vec(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	// Material		grey = set_material(set_vec(0.2f, 0.2f, 0.2f), set_vec(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	Material		red = set_material(set_vec(1.0f, 0.32f, 0.36f), set_vec(0.0f, 0.0f, 0.0f), 1.0f, 255.0f);
	// Material		blue = set_material(set_vec(0.65f, 0.77f, 0.97f), set_vec(0.0f, 0.0f, 0.0f), 1.0f, 255.0f);
	// Material		yellow = set_material(set_vec(0.9f, 0.76f, 0.46f), set_vec(0.0f, 0.0f, 0.0f), 1.0f, 255.0f);

	Material		light = set_material(set_vec(0.0f, 0.0f, 0.0f), set_vec(1.0f, 1.0f, 1.0f), 0.0f, 0.0f);

	init_objects(3, &(data->objects));
	data->objects.objects[0] = set_plane(set_vec(0.0f, -4.0f, 0.0f), set_vec(0.0f, 1.0f, 0.0f), white);
    data->objects.objects[1] = set_light(set_vec(0.0f, 20.0f, -30.0f), 3.0f, light);

	data->objects.objects[2] = set_cylinder(set_vec(0.0f, 30.0f, -30.0f), set_vec(0.0f, 1.0f, 0.0f), 4, red);

    // data->objects.objects[1] = set_sphere(set_vec(0.0f, 0.0, -20.0f), 4.0f, red);
    // data->objects.objects[2] = set_sphere(set_vec(5.0f, -1.0f, -15.0f), 2.0f, yellow);
    // data->objects.objects[3] = set_sphere(set_vec(5.0f, 0.0f, -25.0f), 3.0f, blue);
    // data->objects.objects[4] = set_sphere(set_vec(-5.5f, 0.0f, -15.0f), 3.0f, white);
    // data->objects.objects[5] = set_light(set_vec(-50.0f, 10.0f, -30.0f), 3.0f, light);
    // data->objects.objects[6] = set_light(set_vec(50.0f, 20.0f, -30.0f), 3.0f, light);

}

void				quit(t_data *data)
{
	SDL_FreeSurface(data->surf);

	free_objects(&(data->objects));
}

void				display(t_data *data)
{
	data->tex = SDL_CreateTextureFromSurface(data->esdl->en.ren, data->surf);
	SDL_RenderClear(data->esdl->en.ren);
	SDL_RenderCopy(data->esdl->en.ren, data->tex, NULL, NULL);
	SDL_RenderPresent(data->esdl->en.ren);
	SDL_DestroyTexture(data->tex);
}

int					main(int argc, char **argv)
{
	t_data			data;
	t_esdl			esdl;

	data.esdl = &esdl;
	if (esdl_init(&esdl, 640, 480, "RTV1") == -1)
		return (-1);
	init(&data);


	unsigned int start, end;
	start = SDL_GetTicks();

	render(&data);

	end = SDL_GetTicks();
	printf("took %d\n", end - start);

	while (esdl.run)
	{
		esdl_update_events(&esdl.en.in, &esdl.run);

		display(&data);

		esdl_fps_limit(&esdl);
		esdl_fps_counter(&esdl);
	}
	quit(&data);
	esdl_quit(&esdl);
	(void)argc;
	(void)argv;
	return (0);
}
