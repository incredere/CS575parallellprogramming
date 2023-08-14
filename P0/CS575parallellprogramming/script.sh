#!/bin/csh

# number of threads:
foreach t  ( 1 4)
    echo NUMT = $t
    # number of subdivisions:
    foreach s (5 10 100 1000 10000 100000 500000 )
        echo NUMTRIES = $s
        g++   -DNUMTRIES=$s   -DNUMT=$t   simple.cpp  -o prog -lm  -fopenmp
        ./prog
    end
end