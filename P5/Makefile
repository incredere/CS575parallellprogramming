CUDA_PATH	=	/usr/local/apps/cuda/cuda-10.1
CUDA_BIN_PATH	=	$(CUDA_PATH)/bin
CUDA_NVCC	=	$(CUDA_BIN_PATH)/nvcc

montecarlo:	montecarlo.cu
		$(CUDA_NVCC) -o montecarlo  montecarlo.cu

arrayMul:	arrayMul.cu
		$(CUDA_NVCC) -o arrayMul  arrayMul.cu

proj05:	proj05.cu
		$(CUDA_NVCC) -o p5  p5.cu
