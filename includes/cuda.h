#ifndef CUDA_H
# define CUDA_H

#include <header.h>

typedef struct	s_cuda
{
	Uint32		*screen;
	int			rx;
	int			ry;
	dim3		blocksize;
	dim3		gridsize;
	int			bx;
	int			by;
}				t_cuda;

void			do_cuda(t_data *data, int (*f)(t_data *, t_cuda *));

#endif