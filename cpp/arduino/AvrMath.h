//#include <math.h>
#pragma once

#define constrain(x,l,h) ((x)<(l)?(l):((x)>(h)?(h):(x)))
#define map(x,inMin,inMax,outMin,outMax) (((x)-(inMin))*((outMax)-(outMin))/((inMax)-(inMin))+outMin)

#ifdef abs
#undef abs
#endif
#define abs(x) ((x)>0?(x):-(x))

#ifdef max
#undef max
#endif
#define max(a,b) ((a)>(b)?(a):(b))

#ifdef min
#undef min
#endif
#define min(a,b) ((a)<(b)?(a):(b))
float pow(float, float);
#define sq(x) ((x)*(x))
float sqrt(float);

// https://stackoverflow.com/a/26091248/2063546
#define floor(x) ((x) < (int)(x) ? (int)(x) - 1 : (int)(x))
#define ceil(x) (0-floor(-(x)))
// https://stackoverflow.com/a/26342944/2063546
#define fmod(a,b) ((a)-(b)*floor((a)/(b)))

#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)


// http://www.ganssle.com/approx.htm
// http://www.ganssle.com/approx/sincos.cpp
double cos(double);
double sin(double);
double tan(double);

