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

Vec3f				raytrace(t_vec rayorig, t_vec raydir, t_data *data)
{
	float			tnear;
	float			t0;
	float			t1;

	Sphere			*sphere = NULL;

	Material		mat = set_material(set_vec(1.0f, 1.0f, 1.0f), set_vec(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	Sphere			spheres = set_sphere(set_vec(0.0f, 0.0f, -20.0f), 4, mat);

	tnear = INFINITY;

		t0 = INFINITY;
		t1 = INFINITY;
		if (sphere_intersect(rayorig, raydir, spheres, &t0, &t1))
		{
			if (t0 < 0)
				t0 = t1;
			if (t0 < tnear)
			{
				tnear = t0;
				sphere = &(spheres);
			}
		}

	if (sphere == NULL)
		return (set_vec(0.0f, 0.0f, 0.0f));
	return (set_vec(1.0f, 1.0f, 1.0f));
	(void)data;
}

void				render(t_data *data)
{
	float invWidth = 1.0f / (float)SDL_RX;
	float invHeight = 1.0f / (float)SDL_RY;
	float fov = 30.0f;
	float aspectratio = SDL_RX / (float)SDL_RY;
	float angle = tan(M_PI * 0.5f * fov / 180.0f);

	t_vec camera_pos = set_vec(0.0f, -0.2f, 0.0f);

	for (int y = 0; y < SDL_RY; y++)
	{
		for (int x = 0; x < SDL_RX; x++)
		{
            float xx = (2.0f * ((x + 0.5f) * invWidth) - 1.0f) * angle * aspectratio;
            float yy = (1.0f - 2.0f * ((y + 0.5f) * invHeight)) * angle;
            t_vec raydir = {xx, yy, -1};

            raydir = vec_add(&raydir, &camera_pos);

            raydir = vec_normalize(&raydir);
            t_vec rayorig = {0, 5, 10.0f};

            Vec3f vec_color = raytrace(rayorig, raydir, data);
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

	init_spheres(1, &(data->spheres));
}

void				quit(t_data *data)
{
	SDL_FreeSurface(data->surf);

	free_spheres(&(data->spheres));
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

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);

	render(&data);

	clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
	printf("%" PRId64 "\n", delta_us);

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
