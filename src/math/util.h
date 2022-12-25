// this file includes both general util and color util

#ifndef RAY_TRACER_UTIL_H
#define RAY_TRACER_UTIL_H

#include <cmath>
#include <random>
#include <limits>
#include <memory>
#include <iostream>
#include "ray.h"
#include "vec3.h"
#include <thread>
#include <vector>

/*
using std::shared_ptr;
using std::make_shared;
using std::sqrt;
*/
using namespace std;

const double infinity = numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double rand_unit()
{
  return rand() / (RAND_MAX + 1.0);
}

inline static vec3 vrandom()
{
  return vec3(rand_unit(), rand_unit(), rand_unit());
}
inline static double rand_range(double lb, double hb)
{
  return (rand_unit() * hb) + lb;
}
inline static vec3 vrandom(double lb, double hb)
{
  return vec3(rand_range(lb, hb), rand_range(lb, hb), rand_range(lb, hb));
}

inline static vec3 random_in_unit_disk()
{
  while (true)
  {
    vec3 p = vec3(rand_range(-1, 1), rand_range(-1, 1), 0);
    if (p.length_sqaure() >= 1)
      continue;
    return p;
  }
}

double clamp(double x, double min, double max)
{
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

inline double degrees_to_radians(double degrees)
{
  return degrees * pi / 180.0;
}

// this helper map a decimal to rgb value
void write_color(ostream &out, color pixel_color)
{
  // cout<<pixel_color.x();
  out << static_cast<int>(255.99 * pixel_color.x()) << ' '
      << static_cast<int>(255.99 * pixel_color.y()) << ' '
      << static_cast<int>(255.99 * pixel_color.z()) << '\n';
}

void write_color(ostream &out, color pixel_color, int samples_per_pixel)
{
  double r = pixel_color.x();
  double g = pixel_color.y();
  double b = pixel_color.z();
  double scale = 1.0 / samples_per_pixel;
  r *= scale;
  g *= scale;
  b *= scale;
  out << static_cast<int>(255.99 * clamp(r, 0.0, 0.999)) << ' '
      << static_cast<int>(255.99 * clamp(g, 0.0, 0.999)) << ' '
      << static_cast<int>(255.99 * clamp(b, 0.0, 0.999)) << '\n';
}

vec3 random_in_unit_sphere()
{
  vec3 p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
  while (p.norm() >= 1.0)
  {
    p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
  }
  return p;
}

// a,b,c are column vectors in 3*3 matrix
double det_3(vec3 a, vec3 b, vec3 c)
{
  double res = 0;
  res += a[0] * (b[1] * c[2] - b[2] * c[1]);
  res -= b[0] * (a[1] * c[2] - a[2] * c[1]);
  res += c[0] * (a[1] * b[2] - a[2] * b[1]);
  return res;
}
double min3(double a, double b, double c)
{
  return min(a, min(b, c));
}
double max3(double a, double b, double c)
{
  return max(a, max(b, c));
}
#endif