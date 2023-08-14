import os
for x in [64, 128, 256, 512, 1024, 3072, 4096, 8196]:
    for y in [1,2,4,8,16,32,48]:
        cmd = "g++ -DNUMT=%d -DNUMNODES=%d p2.cpp -o p2 -lm  -fopenmp" % (y, x)
        os.system(cmd)
        cmd = "./p2"
        os.system(cmd)
