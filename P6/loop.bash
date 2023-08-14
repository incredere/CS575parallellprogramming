#!/bin/bash
echo "NMB,LOCAL_SIZE,GigaMultsPerSecond,NUM_WORK_GROUPS"
for t in 1024 4096 16384 65536 262144 1048576 2097152 4194304 8388608
do
        for b in 8 16 32 128 256 512
        do
                g++ /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -DNMB=$t -DLOCAL_SIZE=$b -o MultReduce MultReduce.cpp -lm -fopenmp
                ./MultReduce
        done
done