#include <complex>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <iostream>
#include <fstream>

using std::complex;
using std::vector;
using std::string;
using std::ofstream;
using std::endl;
using std::max;

#define M_PI 3.14159265358979323846
#define ITERLIM 50
#define PHISPLIT 60
#define PIXELS 1200
#define NIMAGES 1

const complex<double> i (0, 1);
const double dphi = 2 * M_PI / (double) PHISPLIT;
const double t0 = -3 * M_PI, t1 = 3 * M_PI;
const double dt = (t1 - t0) / NIMAGES;

// define a path in a cartesian product of complex planes
// through which a series of julia sets will be plotted
// the vector returned must be ordered from as (an, ..., a0)
// KEEP THE LEADING COEFFICIENT FAR AWAY FROM 0
vector<complex<double>> path(double t) {
  return {
    complex<double>(sin(t) / 10,0.5),
    complex<double>(0.01*t*cos(t), 0.001*sin(t)),
    complex<double>(cos(t), -sin(2*t)),
    complex<double>(-sin(t)/10, t/10)
  };
}

// theoretic convergence limit for a specific polynomial
double theoreticEps(const vector<complex<double>> &coefs) {
  int n = coefs.size();
  double an = abs(coefs[0]);
  double sum = 0;
  for (int i = 1; i < n; i++) sum += abs(coefs[i]);
  return max(
    max(1.0, 2 * sum / an), 
    pow(2 * 1.0001 / an, 1 / (double) (n - 2)))
  ;
}

inline complex<double> horner(
  const vector<complex<double>> &coefs, 
  const complex<double> &z
) {
  complex<double> sum (0, 0);
  for (const complex<double> &a_i : coefs)
    sum = sum * z + a_i;
  return sum;
}

int convergance(
  complex<double> z, 
  const vector<complex<double>> &coefs, 
  const double &eps
) {
  int count = 1;
  while (count < ITERLIM && abs(z) <= eps) {
    z = horner(coefs, z);
    count++;
  }
  return count;
}

// convergence limit based on a test simulation
double simulatedEps(
  const vector<complex<double>> &coefs, 
  const double &itereps
) {
  double maxradius = 0;
  double r = 2 * itereps / (double) PIXELS;
  for (double phi = 0; phi < 2 * M_PI; phi += dphi) {
    complex<double> z;
    complex<double> dz = r * exp(i * phi);
    complex<double> z0 = (double) PIXELS * dz;
    for (z = z0; abs(z) > r; z -= dz) {
      int count = convergance(z, coefs, itereps);
      if (count > 2) break;
    }
    if (abs(z) > maxradius) maxradius = abs(z) + r;
  }
  return maxradius;
}

inline complex<double> coordTranslate(
  const int &i, const int &j, const double &eps
) {
  double x = 2 * eps * (double) i / PIXELS - eps;
  double y = 2 * eps * (double) j / PIXELS - eps;
  return complex<double> (x, y);
}

inline void ppmBasicColorStream(ofstream &ppm, const int &count) {
  double ratio = (count == ITERLIM) ? 0 : (double) count / ITERLIM;
  if (count == 1) ratio = (double) 2 / ITERLIM; // simulation's gradient fix
  ppm << std::floor(ratio * 255) << " 0 0  ";
}

void writeJuliaPpm(
  const vector<complex<double>> &coefs, 
  const double &eps,
  const string &filename
) {
  ofstream ppm;
  ppm.open(filename);

  ppm << "P3" << endl;
  ppm << PIXELS << " " << PIXELS << endl;
  ppm << 255 << endl;

  for (int j = 0; j < PIXELS; j++) {
    for (int i = 0; i < PIXELS; i++) {
      complex<double> z = coordTranslate(i, j, eps);
      int count = convergance(z, coefs, eps);
      ppmBasicColorStream(ppm, count);
    }
    ppm << endl;
  }

}

double staticEps( void ) {
  double t = t0, eps = 0;
  for (int i = 0; i < NIMAGES; i++) {
    t += dt; 
    const vector<complex<double>> coefs = path(t);
    double teps = theoreticEps(coefs);
    double seps = simulatedEps(coefs, teps);
    if (eps < seps) eps = seps;
  }
  return eps;
}

void imageSeries(bool static_img) {
  double eps = (static_img) ? staticEps() : 0;
  double t = t0;
  for (int i = 0; i < NIMAGES; i++) {
    t += dt;

    string stri = std::to_string(i);
    string filename = "images/julia_" + stri + ".ppm";

    const vector<complex<double>> coefs = path(t);
    if (!static_img) {
      double teps = theoreticEps(coefs);
      double seps = simulatedEps(coefs, teps);
      writeJuliaPpm(coefs, seps, filename);
    } else writeJuliaPpm(coefs, eps, filename);
    
    std::cout << i << endl;
  }
}

int main( void ) {
  imageSeries(false);
  return 0;
}
