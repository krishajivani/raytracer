#ifndef RAY_H
#define RAY_H
#include <iostream>
#include "vec3.h"

class ray
{
public:
  vec3 origin;
  // direction
  vec3 velocity;
  ray();
  ray(vec3 a, vec3 b);
  // vec3 get_origin();
  // vec3 get_velocity();
  vec3 point_at_time(double t);
};

#endif