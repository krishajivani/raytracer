#include <iostream>
#include <cmath>
#include "ray.h"

ray::ray()
{
  origin = vec3();
  velocity = vec3();
}
ray::ray(vec3 a, vec3 b)
{
  origin = a;
  velocity = b;
}

// vec3 ray::get_origin() { return origin; }

// vec3 ray::get_velocity() { return velocity; }

vec3 ray::point_at_time(double t)
{
  point3 ans = origin + velocity * t;
  return ans;
}