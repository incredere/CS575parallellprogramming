#!/bin/bash
echo "NumberofTrials,Blocksize,MegaTrials/Second,Probability"
for t in 1024 4096 16384 65536 262144 1048576 2097152 4194304 8388608 16777216
do
        for b in 8 32 128
        do
                 /usr/local/apps/cuda/cuda-10.1/bin/nvcc -DNUMTRIALS=$t -DBLOCKSIZE=$b -o p5 p5.cu
                ./p5
        done
done