#ifndef HITTABLE_H
#define HITTABLE_H
#include <memory>
#include "ray.h"
#include "aabb.h"

//#include "../render/material.h"

class material;
struct hit_record
{
  point3 p;
  // normal unit vector to the surface;
  // we assume it points "out"
  vec3 normal;
  double t;
  int u, v;
  bool texture = false;
  // if the ray and the normal face in the same direction,
  // the ray is inside the object; otherwise, outside
  bool front_face;
  std::shared_ptr<material> mat_ptr;
  // color texture_color;
  // bool has_texture = false;

  inline void set_face_normal(ray r, vec3 outward_normal)
  {
    front_face = dot(r.velocity, outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable
{
public:
  virtual bool hit(ray r, double t_min, double t_max, hit_record &rec) = 0;
  virtual bool bounding_box(aabb &output_box) = 0;
};
#endif