#ifndef AABB_HH
#define AABB_HH
#include "vec3.h"
#include <cmath>

class aabb
{
public:
  point3 minimum;
  point3 maximum;

  aabb()
  {
    minimum = vec3();
    maximum = vec3();
  }
  aabb(point3 v1, point3 v2)
  {
    minimum = v1;
    maximum = v2;
  }
  bool hit(ray r, double t_min, double t_max)
  {
    vec3 ori = r.origin;
    vec3 invD = vec3(1 / r.velocity.x(), 1 / r.velocity.y(), 1 / r.velocity.z());
    vec3 lbound = (minimum - ori) * invD;
    vec3 rbound = (maximum - ori) * invD;
    for (int i = 0; i < 3; i++)
    {
      double t0 = std::min(lbound[i], rbound[i]);
      double t1 = std::max(lbound[i], rbound[i]);
      double tmin = std::max(t_min, t0);
      double tmax = std::min(t_max, t1);
      if (tmax < tmin)
      {
        return false;
      }
    }
    return true;
  }
};

#endif