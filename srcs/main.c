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

int
intersectPlane(Ray ray, Vec3f pos, Vec3f normal, float *t)
{
	float denom;

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

Object
set_plane(Vec3f pos, Vec3f norm, Material mat)
{
	Object	object;

	object.objtype = PLANE;
	object.pos = pos;
	object.norm = norm;
	object.mat = mat;
	return (object);
}

float			calculateLambert(t_vec lightDirection, t_vec nhit)
{
	return (max(0.0f, dot_product(lightDirection, nhit)));
}

float calculatePhong(Vec3f lightDirection, Vec3f phit, Vec3f nhit, Ray ray, Material mat)
{
	t_vec viewDirection = vec_sub(phit, ray.pos);
	viewDirection = vec_normalize(viewDirection);


	t_vec blinnDirection = vec_sub(lightDirection, viewDirection);
	blinnDirection = vec_normalize(blinnDirection);

	float blinnTerm = max(dot_product(blinnDirection, nhit), 0.0f);
	return mat.spec_value * powf(blinnTerm, mat.spec_power);
}

Vec3f				raytrace(Ray *ray, t_data *data)
{
	float			tnear;
	float			t0;
	float			t1;

	Object			*current_obj = NULL;


	tnear = INFINITY;
	for (size_t i = 0; i < data->objects.nb_obj; i++)
	{
		t0 = INFINITY;
		t1 = INFINITY;
		if (data->objects.objects[i].objtype == SPHERE && sphere_intersect(ray, data->objects.objects[i], &t0, &t1))
		{
			if (t0 < 0)
				t0 = t1;
			if (t0 < tnear)
			{
				tnear = t0;
				current_obj = &(data->objects.objects[i]);
			}
		}
		else if (data->objects.objects[i].objtype == PLANE && intersectPlane(*ray, data->objects.objects[i].pos, data->objects.objects[i].norm, &t0))
		{
			if (t0 < tnear)
			{
				tnear = t0;
				current_obj = &(data->objects.objects[i]);
			}
		}
	}

	if (current_obj == NULL)
		return (set_vec(0.0f, 0.0f, 0.0f));


	t_vec phit = vec_add(ray->pos, vec_mult_f(ray->dir, tnear));
	t_vec nhit = vec_sub(phit, current_obj->pos);
	nhit = vec_normalize(nhit);

	Vec3f color = set_vec(0.0f, 0.0f, 0.0f);
	for (size_t i = 0; i < data->objects.nb_obj; i++)
	{
		if (data->objects.objects[i].is_light == 1)
		{
			int shadowed = 1;
			Object current_light = data->objects.objects[i];

			t_vec lightDirection = vec_sub(current_light.pos, phit);
			lightDirection = vec_normalize(lightDirection);

			for (size_t j = 0; j < data->objects.nb_obj; j++)
			{
				if (i != j)
				{
					Object tmp_obj = data->objects.objects[j];
					Ray ray_tmp = {vec_add(phit, nhit), lightDirection};
					if (tmp_obj.objtype == SPHERE && sphere_intersect(&ray_tmp, tmp_obj, &t0, &t1) == 1)
					{
						shadowed = 0;
						break ;
					}
					ray_tmp.pos = vec_add(phit, tmp_obj.norm);
					if (tmp_obj.objtype == PLANE && intersectPlane(ray_tmp, tmp_obj.pos, tmp_obj.norm, &t0) == 1)
					{
						shadowed = 0;
						break ;
					}
				}
			}

			float lambert = 0, phongTerm = 0;
			if (shadowed == 1)
			{
				lambert = calculateLambert(lightDirection, nhit);
				phongTerm = calculatePhong(lightDirection, phit, nhit, *ray, current_obj->mat);

			}
			color = vec_add(color, vec_mult( current_obj->mat.surf_color, vec_add( vec_mult_f( current_light.mat.emis_color, lambert), vec_mult_f( current_obj->mat.surf_color, phongTerm ) ) ));

		}
	}
	return (vec_mult(current_obj->mat.surf_color, color));
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

	Material		white = set_material(set_vec(0.9f, 0.9f, 0.9f), set_vec(0.0f, 0.0f, 0.0f), 5.0f, 100.0f);
	//Material		grey = set_material(set_vec(0.2f, 0.2f, 0.2f), set_vec(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	Material		red = set_material(set_vec(1.0f, 0.32f, 0.36f), set_vec(0.0f, 0.0f, 0.0f), 5.0f, 100.0f);
	Material		blue = set_material(set_vec(0.65f, 0.77f, 0.97f), set_vec(0.0f, 0.0f, 0.0f), 5.0f, 100.0f);
	Material		yellow = set_material(set_vec(0.9f, 0.76f, 0.46f), set_vec(0.0f, 0.0f, 0.0f), 5.0f, 100.0f);

	Material		light = set_material(set_vec(0.0f, 0.0f, 0.0f), set_vec(2.0f, 2.0f, 2.0f), 0.0f, 0.0f);

	init_objects(8, &(data->objects));
	data->objects.objects[0] = set_plane(set_vec(0.0f, -3.0f, 0.0f), set_vec(0.0f, 1.0f, 0.0f), white);
    data->objects.objects[1] = set_sphere(set_vec(0.0f, 0.0, -20.0f), 4.0f, red);
    data->objects.objects[2] = set_sphere(set_vec(5.0f, -1.0f, -15.0f), 2.0f, yellow);
    data->objects.objects[3] = set_sphere(set_vec(5.0f, 0.0f, -25.0f), 3.0f, blue);
    data->objects.objects[4] = set_sphere(set_vec(-5.5f, 0.0f, -15.0f), 3.0f, white);
    data->objects.objects[5] = set_light(set_vec(-50.0f, 20.0f, -30.0f), 3.0f, light);
    data->objects.objects[6] = set_light(set_vec(50.0f, 20.0f, -30.0f), 3.0f, light);
    data->objects.objects[7] = set_light(set_vec(0.0f, 20.0f, -30.0f), 3.0f, light);

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


	struct timeval stop, start;
	gettimeofday(&start, NULL);

	render(&data);

	gettimeofday(&stop, NULL);
	printf("took %ld\n", stop.tv_usec - start.tv_usec);

	display(&data);


	while (esdl.run)
	{
		esdl_update_events(&esdl.en.in, &esdl.run);

		esdl_fps_limit(&esdl);
		esdl_fps_counter(&esdl);
	}
	quit(&data);
	esdl_quit(&esdl);
	(void)argc;
	(void)argv;
	return (0);
}
