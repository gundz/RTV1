/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgundlac <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/09/17 16:00:29 by fgundlac          #+#    #+#             */
/*   Updated: 2016/09/17 16:00:30 by fgundlac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
# define HEADER_H

# include <easy_sdl.h>
# include <vec.h>
# include <time.h>

typedef struct			s_material
{
	Vec3f				surf_color;
	Vec3f				emis_color;
	float				spec_value;
	float				spec_power;
}						t_material;
typedef t_material 		Material;

typedef struct			s_sphere
{
	Vec3f				pos;
	float				rad;
	Material			mat;
	int					is_light;
}						t_sphere;
typedef t_sphere		Sphere;

typedef struct			s_spheres
{
	size_t				nb_spheres;
	Sphere				*spheres;
}						t_spheres;
typedef t_spheres		Spheres;

typedef struct			s_data
{
	Spheres				spheres;
	t_esdl				*esdl;
	SDL_Surface			*surf;
	SDL_Texture			*tex;
}						t_data;

#endif
