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
intersectPlane(Vec3f n, Vec3f p0, Vec3f l0, Vec3f l, float *t)
{
	float denom;
    // assuming vectors are all normalized
    denom = dot_product(&n, &l);

    if (denom > 1e-6)
    {
        Vec3f p0l0 = vec_sub(&p0, &l0);
        *t = dot_product(&p0l0, &n) / denom;
        return (*t >= 0);
    }
    return (0);
}

Vec3f				raytrace(Ray *ray, t_data *data)
{
	float			tnear;
	float			t0;
	float			t1;

	Sphere			*current_sphere = NULL;


	tnear = INFINITY;
	for (size_t i = 0; i < data->spheres.nb_spheres; i++)
	{
		t0 = INFINITY;
		t1 = INFINITY;
		if (sphere_intersect(ray, data->spheres.spheres[i], &t0, &t1))
		{
			if (t0 < 0)
				t0 = t1;
			if (t0 < tnear)
			{
				tnear = t0;
				current_sphere = &(data->spheres.spheres[i]);
			}
		}
	}

	float tt;
	int ret = intersectPlane(vec_normalize(&(data->plane_norm)), data->plane_pos, ray->pos, ray->dir, &tt);
	if (ret == 1)
		return (set_vec(0.5f, 0.5f, 0.0f));

	if (current_sphere == NULL)
		return (set_vec(0.0f, 0.0f, 0.0f));

	return (current_sphere->mat.surf_color);
}

void				bite(t_data *data)
{
	if (data->esdl->en.in.key[SDL_SCANCODE_Q] == 1)
		data->plane_norm.x += 0.1f;
	if (data->esdl->en.in.key[SDL_SCANCODE_W] == 1)
		data->plane_norm.y += 0.1f;
	if (data->esdl->en.in.key[SDL_SCANCODE_E] == 1)
		data->plane_norm.z += 0.1f;

	if (data->esdl->en.in.key[SDL_SCANCODE_A] == 1)
		data->plane_norm.x -= 0.1f;
	if (data->esdl->en.in.key[SDL_SCANCODE_S] == 1)
		data->plane_norm.y -= 0.1f;
	if (data->esdl->en.in.key[SDL_SCANCODE_D] == 1)
		data->plane_norm.z -= 0.1f;

	if (data->esdl->en.in.key[SDL_SCANCODE_T] == 1)
		data->plane_pos.x += 0.1f;
	if (data->esdl->en.in.key[SDL_SCANCODE_Y] == 1)
		data->plane_pos.y += 0.1f;
	if (data->esdl->en.in.key[SDL_SCANCODE_U] == 1)
		data->plane_pos.z += 0.1f;

	if (data->esdl->en.in.key[SDL_SCANCODE_G] == 1)
		data->plane_pos.x -= 0.1f;
	if (data->esdl->en.in.key[SDL_SCANCODE_H] == 1)
		data->plane_pos.y -= 0.1f;
	if (data->esdl->en.in.key[SDL_SCANCODE_J] == 1)
		data->plane_pos.z -= 0.1f;

	printf("%f %f %f\n", data->plane_norm.x, data->plane_norm.y, data->plane_norm.z);
}

void				render(t_data *data)
{
	float invWidth = 1.0f / (float)SDL_RX;
	float invHeight = 1.0f / (float)SDL_RY;
	float fov = 30.0f;
	float aspectratio = SDL_RX / (float)SDL_RY;
	float angle = tan(M_PI * 0.5f * fov / 180.0f);

	t_vec camera_pos = set_vec(0.0f, 0.0f, 0.5f);

	bite(data);

	for (int y = 0; y < SDL_RY; y++)
	{
		for (int x = 0; x < SDL_RX; x++)
		{
            float xx = (2.0f * ((x + 0.5f) * invWidth) - 1.0f) * angle * aspectratio;
            float yy = (1.0f - 2.0f * ((y + 0.5f) * invHeight)) * angle;
            t_vec raydir = {xx, yy, -1};

            raydir = vec_add(&raydir, &camera_pos);

            raydir = vec_normalize(&raydir);
            t_vec rayorig = {0.0f, 0.0f, 0.0f};

            Ray ray = {rayorig, raydir};
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

	Material		white = set_material(set_vec(1.0f, 1.0f, 1.0f), set_vec(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
	Material		red = set_material(set_vec(1.0f, 0.0f, 0.0f), set_vec(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);

	init_spheres(2, &(data->spheres));
	data->spheres.spheres[0] = set_sphere(set_vec(0.0f, 0.0f, -20.0f), 4, white);
    data->spheres.spheres[1] = set_sphere(set_vec(5.0f, -1.0f, -15.0f), 2, red);

	data->plane_pos = set_vec(0.0f, 0.0f, 0.0f);
	data->plane_norm = set_vec(0.0f, -15.0f, 4.0f);
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


	struct timeval stop, start;
	gettimeofday(&start, NULL);

	render(&data);

	gettimeofday(&stop, NULL);
	printf("took %d\n", stop.tv_usec - start.tv_usec);

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
