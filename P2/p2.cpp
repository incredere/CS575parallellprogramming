#include <omp.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
using namespace std;

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

#ifndef DEBUG
#define DEBUG    false
#endif

#ifndef NUMT
#define NUMT     4
#endif

#ifndef NUMTRIES
#define NUMTRIES 10
#endif

#ifndef NUMNODES
#define NUMNODES 64
#endif

const float N = 2.5f;
const float R = 1.2f;

float
Height( int iu, int iv )    // iu,iv = 0 .. NUMNODES-1
{
    float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);    // -1. to +1.
    float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);    // -1. to +1.

    float xn = pow( fabs(x), (double)N);
    float yn = pow( fabs(y), (double)N);
    float rn = pow( fabs(R), (double)N);
    float r = rn - xn - yn;
    if( r <= 0. )
            return 0.;
    float height = pow( r, 1./(double)N );
    return height;
}


// The main Program
float Height( int, int );    // function prototype

int main(int argc, char *argv[])
{

    // the area of a single full-sized tile:
    omp_set_num_threads(NUMT);
    float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
                ( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );
    float volume = 0.0;
    
    double mmVolumeCalcsPerSec = 0.;
    for (int t = 0; t < NUMTRIES; t++)
    {
        double time0 = omp_get_wtime();
        // sum up the weighted heights into the variable "volume"
        // using an OpenMP for loop and a reduction:
        #pragma omp parallel for reduction(+:volume)
        for (int i = 0; i < NUMNODES*NUMNODES; i++)
        {

            int iu = i % NUMNODES;
            int iv = i / NUMNODES;
            float z = Height( iu, iv );
            
            float area = 1;

            if (iu == 0 || iu == NUMNODES - 1)
            {
                area *= .5;
            }
            if (iv == 0 || iv == NUMNODES - 1)
            {
                area *= .5;
            }
            volume += fullTileArea*z*area;
        }
        
        
        double time1 = omp_get_wtime();
        double mmcalcsPerSec = (double)NUMNODES*NUMNODES / (time1 - time0)/1000000.;
        mmVolumeCalcsPerSec += mmcalcsPerSec;
    }
    printf("%d,%d,%f,%f\n", NUMT, NUMNODES, 2*volume/(double)NUMTRIES, mmVolumeCalcsPerSec/(double)NUMTRIES);
    return 0;
}
