#include <complex>
#include <vector>
#include <iostream>
#include "include/julia_polynomial_series.h"
#include "include/color_mixer.h"

using std::complex;
using std::vector;

#define M_PI 3.14159265358979323846

// The ceofficients returned are ordered as (an, ..., a0)
// The leading coefficient must be NOWHERE NEAR 0!
complex_polynomial path(double t) {
  return {
    complex<double>(1, 0),
    complex<double>(0, 0),
    complex<double>(t, 0)
  };
}

const vector<color> base_colors {
  {30, 45, 60},
  {153, 255, 255},
  {10, 20, 30},
  {255, 128, 0},
  {102, 178, 255},
  {255,0,255},
  {204, 204, 255},
  {51, 255, 51},
  {153, 255, 255}
};

int main( void ) {
  vector<color> gradient = color_mixer::makeGradient(
    base_colors, Julia::itercount
  );
  int pixels = 2000;
  int nframes = 400;
  double a = -2, b = 1.5;
  Julia julia (
    "temp_imgs", 
    nframes, pixels, 
    a, b, &path, 
    gradient
  );
  julia.dynamicImageSeries();
  return 0;
}