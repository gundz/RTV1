#include <cuda.h>

void			do_cuda(t_data *data, int (*f)(t_data *, t_cuda *))
{
	static t_cuda	cuda = {NULL};
	const size_t	size = SDL_RY * SDL_RX * data->surf->format->BytesPerPixel;

	if (cuda.screen == NULL)
	{
		cudaMalloc((void **)&cuda.screen, size);
		cuda.blocksize = dim3(32, 32);
		cuda.bx = (SDL_RX + cuda.blocksize.x - 1) / cuda.blocksize.x;
		cuda.by = (SDL_RY + cuda.blocksize.y - 1) / cuda.blocksize.y;
		cuda.gridsize = dim3(cuda.bx, cuda.by);
		cuda.rx = SDL_RX;
		cuda.ry = SDL_RY;
	}

	f(data, &cuda);
	cudaDeviceSynchronize();

	SDL_LockSurface(data->surf);
	cudaMemcpy(data->surf->pixels, cuda.screen, size, cudaMemcpyDeviceToHost);
	SDL_UnlockSurface(data->surf);

	if (data->esdl->run == 0)
		cudaFree(cuda.screen);
}