
#CONFIG
	SRCS =			main.c \
					vec_tools.c \
					spheres.c \
					plane.c \
					material.c \
					light.c \

	#CUDA CONFIG
	SRCS_NO_CUDA =	\

	SRCS_CUDA =		cuda.cu \

ifeq ($(shell uname), Darwin)
	NVCC =			nvcc
	CUDA_FLAGS =	-L /Developer/NVIDIA/CUDA-7.5/lib -lcudart -lstdc++
else
	NVCC =			/usr/local/cuda/bin/nvcc
	CUDA_FLAGS =	-L /usr/local/cuda/lib64 -lcudart -lcuda -lstdc++
endif

	#END CUDA CONFIG

	NAME =			RTV1

	CC =			gcc

	EXTENTION =		c

	CFLAGS = 		-Wall -Werror -Wextra -g

#ADVANCED CONFIG
	SRC_PATH =		./srcs/
	INC_PATH =		./includes/
	OBJ_PATH =		./obj/
	#LIB CONFIG
		#TYPE OF BINARY: LIB or PROGRAM
		TYPE = PROGRAM
	#LIB that are make with the same makefile
		LIB_NAMES = -lesdl
		LIB_PATH =	./ESDL_Lib/
	#OTHER LIB
		LIB_SUPP = `sdl2-config --libs` -lm
		LIB_SUPP_INC = `sdl2-config --cflags`
	#TEXT
		COMPILING_OBJECTS = "\033[4;7mCompiling Objects:\033[0m [$(NAME)]\n"
		COMPILING_BINARY = "\033[4;7mCompiling binary:\033[0m [$(NAME)]\n"
		COMPILING_DONE = "\n\033[4m\033[95md\033[93mo\033[32mn\033[96me\033[0m \033[91m!\033[0m\n\n"
		COMILING_PROGRESS = "\033[37;7m+\033[0m"
		COMPILING_CLEAN = "\033[4;7mCleaning:\033[0m [$(NAME)]\n\033[4m\033[95md\033[93mo\033[32mn\033[96me\033[0m \033[91m!\033[0m\n\n"
		COMPILING_FCLEAN = "\033[4;7mFcleaning:\033[0m [$(NAME)]\033[0m\n\033[4m\033[95md\033[93mo\033[32mn\033[96me\033[0m \033[91m!\033[0m\n\n"
		COMPILING_CUDA = "\033[4;7mCompiling CUDA Objects:\033[0m [$(NAME)]\n"

#DO MAGIC
	#CUDA MAGIC
	CUDA_OBJ = $(addprefix $(OBJ_PATH), $(SRCS_CUDA:.cu=.o))
	#END CUDA MAGIC
	OBJ = $(addprefix $(OBJ_PATH), $(SRCS:.$(EXTENTION)=.o))
	INC = $(addprefix -I, $(INC_PATH))
	#LIB
		LIB_INC = $(addprefix -I, $(addsuffix includes, $(LIB_PATH)))
		LIB = $(addprefix -L, $(LIB_PATH))
		INC += $(LIB_INC) $(LIB_SUPP_INC)
		LDFLAGS = $(LIB) $(LIB_NAMES) $(LIB_SUPP)
		EMPTY =

all: libs name $(OBJ) $(CUDA_OBJ) done $(NAME)

#CUDA COMPIL
cuda: all

ifeq ($(MAKECMDGOALS), cuda)
LDFLAGS += $(CUDA_FLAGS)
$(OBJ_PATH)%.o: $(SRC_PATH)cuda/%.cu
	@ $(NVCC) $(INC) -c $< -o $@
else
SRC += SRCS_NO_CUDA
$(OBJ_PATH)%.o: $(SRC_PATH)no_cuda/%.c
	@ $(CC) $(CFLAGS) $(INC) -c $< -o $@
endif
#END CUDA COMPIL

$(NAME): $(OBJ)
	@ printf $(COMPILING_BINARY)
	@ printf $(COMILING_PROGRESS)
ifeq ($(TYPE), LIB)
	@ ar -rc $(NAME) $(OBJ)
	@ ranlib $(NAME)
else
	@ $(CC) $(CFLAGS) $(OBJ) $(CUDA_OBJ) $(LDFLAGS) -o $(NAME)
endif
	@ printf $(COMPILING_DONE)

$(OBJ_PATH)%.o: $(SRC_PATH)%.$(EXTENTION)
	@ printf $(COMILING_PROGRESS)
	@ mkdir -p $(OBJ_PATH) 2> /dev/null
	@ $(CC) $(CFLAGS) $(INC) $(LIB_INC) -c $< -o $@

#COMPILING SUBLIBS
libs:
	@ $(foreach lib, $(LIB_PATH), make -sC $(lib) ;)

clean:
	@ rm -rf $(OBJ_PATH)
	@ printf $(COMPILING_CLEAN)
	@ $(foreach lib, $(LIB_PATH), make clean -sC $(lib) ;) #CLEANING SUBLIBS

fclean: clean
	@ rm -rf $(NAME)
	@ printf $(COMPILING_FCLEAN)
	@ $(foreach lib, $(LIB_PATH), make fclean -sC $(lib) ;) #FCLEANING SUBLIBS

re: fclean all

#TEXT
name:
	@ printf $(COMPILING_OBJECTS)

done:
	@ printf $(COMPILING_DONE)

.PHONY: all clean fclean re libs cuda
