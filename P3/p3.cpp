#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>

//Global Variables
int    NowYear;        // 2022 - 2028
int    NowMonth;        // 0 - 11

float    NowPrecip;        // inches of rain per month
float    NowTemp;        // temperature this month
float    NowHeight;        // grain height in inches
int    NowNumDeer;        // number of deer in the current population


//Basic Time Step Will be One Month
const float GRAIN_GROWS_PER_MONTH =        9.0;
const float ONE_DEER_EATS_PER_MONTH =        1.0;

const float AVG_PRECIP_PER_MONTH =        7.0;    // average
const float AMP_PRECIP_PER_MONTH =        6.0;    // plus or minus
const float RANDOM_PRECIP =            2.0;    // plus or minus noise

const float AVG_TEMP =                60.0;    // average
const float AMP_TEMP =                20.0;    // plus or minus
const float RANDOM_TEMP =            10.0;    // plus or minus noise

const float MIDTEMP =                40.0;
const float MIDPRECIP =                10.0;

//****************************************
const float PESTICIDES_TEMP_MIN         =   40.0;
const float PESTICIDES_TEMP_MAX         =   60.0;
const float PESTICIDES_FREEZE           =   32.0;
const float PESTICIDES_PRECIP           =   7;
float PestAtkpct;


omp_lock_t Lock;
int NumInThreadTeam;
int NumAtBarrier;
int NumGone;

//Functions

// function prototypes:
float       Ranf(unsigned int *seedp, float, float );
int         Ranf(unsigned int *seedp, int, int );
void        TimeOfDaySeed( );
float       SQR( float x );
void        Deer();
void        Grain();
void        Watcher();
void        Pesticides();


// Main

int main ()
{
;
    // starting date and time:
    NowMonth =    0;
    NowYear  = 2022;

    // starting state (feel free to change this if you want):
    NowNumDeer = 1;
    NowHeight =  1.;
    
    PestAtkpct=  0.;

    printf( "Month\t,Year\t,Temp(C)\t,Precipitation(cm)\t,Grain Height\t,Deer\t,Pest attack %\n");



    omp_set_num_threads( 4 );    // same as # of sections
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Deer( );
        }

        #pragma omp section
        {
            Grain( );
        }

        #pragma omp section
        {
            Watcher( );
        }

        #pragma omp section
        {
            Pesticides( );    // your own
        }
    }       // implied barrier -- all functions must return in order
        // to allow any of them to get past here

    return 0;
}

void
Deer( )
{
    int nextNumDeer;
    while( NowYear < 2028 )
    {
        nextNumDeer = NowNumDeer;
        if (NowNumDeer < NowHeight)
        {
            nextNumDeer++;
        }
        else if (NowNumDeer > NowHeight)
        {
            nextNumDeer--;
        }
        
        if( nextNumDeer < 0 )
        {
            nextNumDeer = 0;
        }
    // DoneComputing barrier:
    #pragma omp barrier

    NowNumDeer = nextNumDeer;
    

    // DoneAssigning barrier:
    #pragma omp barrier

    // DonePrinting barrier:
    #pragma omp barrier

    }
}

void
Grain( )
{

    float nextHeight;
    while( NowYear < 2028 )
    {
        nextHeight = NowHeight;

        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );

        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );

        nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

        if (nextHeight < 0.)
        #ifdef Danger
        nextHeight *= (1 - (DangerPct/100));
        #endif

        if (nextHeight < 0)
        {
            nextHeight = 0.;
        }

     // DoneComputing barrier:
    #pragma omp barrier

        NowHeight = nextHeight;

    // DoneAssigning barrier:
    #pragma omp barrier

    // DonePrinting barrier:
    #pragma omp barrier

    }

}

void
Watcher( )
{
    int tempMonth;
    int tempYear;
    float tempTemp;
    float tempPrecip;
    unsigned int seed = 0;

    while( NowYear < 2028 )
    {

        // DoneComputing barrier:
        #pragma omp barrier

        // DoneAssigning barrier:
        #pragma omp barrier
    
        printf( "%d\t,%d\t,%.2f\t,%.2f\t,%.2f\t,%d\t,%.2f\n",
        NowMonth, NowYear, (5./9.)*(NowTemp-32),NowPrecip*2.54, NowHeight, NowNumDeer, PestAtkpct);


        tempMonth = NowMonth + 1;
        tempYear = NowYear;

        if (tempMonth == 12)
        {
            tempYear++;
            tempMonth = 0;
        }

        NowMonth = tempMonth;
        NowYear = tempYear;

        //Recompute Temperature and Precipitation for Next Month
        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        
        NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. )
            NowPrecip = 0.;

        // DonePrinting barrier:
        #pragma omp barrier
        // . . .
    }

}

void Pesticides()
{
  while( NowYear < 2028 )
  {
    float Pesticides = PestAtkpct;

    if (NowTemp <= PESTICIDES_FREEZE)
        Pesticides = 0.0;
    else if (NowTemp >= PESTICIDES_TEMP_MIN && NowTemp <= PESTICIDES_TEMP_MAX && NowPrecip > PESTICIDES_PRECIP)
    {
        Pesticides += 4.0;
    }
    else if (NowTemp >= PESTICIDES_TEMP_MIN && NowPrecip > PESTICIDES_PRECIP)
   {
    Pesticides += 2.0;
   }
    else if (NowTemp < PESTICIDES_TEMP_MIN && NowPrecip > PESTICIDES_PRECIP)
    {
        Pesticides -= 2.0;
    }
   else
    {
        Pesticides -= 4.0;
    }

    if (Pesticides > 100.0)
    {
        Pesticides = 100.0;
    }
    else if (Pesticides < 0.0)
    {
        Pesticides = 0.0;
    }

    // DoneComputing barrier:
    #pragma omp barrier
      PestAtkpct = Pesticides;

    // DoneAssigning barrier:
    #pragma omp barrier

    // DonePrinting barrier:
    #pragma omp barrier

  }
}

//Helper Functions
float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}

void
TimeOfDaySeed( )
{
    struct tm y2k = { 0 };
    y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

    time_t  timer;
    time( &timer );
    double seconds = difftime( timer, mktime(&y2k) );
    unsigned int seed = (unsigned int)( 1000.*seconds );    // milliseconds
    srand( seed );
}

float
SQR( float x )
{
        return x*x;
}
