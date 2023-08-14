#!/bin/csh

#foreach t ( 1 2 4 8 12 16 20 24 32 )
foreach n (1000 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608)
    g++ -DARRAYSIZE=$n pro4.cpp -o pro4  -lm  -fopenmp
./pro4
#end
end
