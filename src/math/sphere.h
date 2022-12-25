#ifndef SPHERE_H
#define SPHERE_H

#include "../render/material.h"
#include "hittable.h"
#include "hittablelist.h"
// #include "aabb.h"
#include "vec3.h"
#include "util.h"
#include <cmath>
#include <memory>

class sphere : public hittable
{
public:
  point3 center;
  double radius;
  shared_ptr<material> mat_ptr;
  // aabb bbox;
  sphere();
  sphere(point3 center, double rad, shared_ptr<material> m);
  virtual bool hit(ray r, double t_min, double t_max, hit_record &rec) override;
  virtual bool bounding_box(aabb &output_box) override;
};

sphere::sphere()
{
  center = vec3();
  radius = 0;
}

sphere::sphere(point3 cen, double rad, shared_ptr<material> m)
{
  center = cen;
  radius = rad;
  mat_ptr = m;
}

bool sphere::bounding_box(aabb &output_box)
{
  point3 v1 = vec3(center.x() - radius,
                   center.y() - radius, center.z() - radius);
  point3 v2 = vec3(center.x() + radius,
                   center.y() + radius, center.z() + radius);
  output_box = aabb(v1, v2);
  return true;
}

bool sphere::hit(ray r, double t_min, double t_max, hit_record &rec)
{
  vec3 oc = r.origin - center;
  double a = r.velocity.length_sqaure();
  double half_b = dot(oc, r.velocity);
  double c = oc.length_sqaure() - radius * radius;

  double discriminant = half_b * half_b - a * c;
  // if there is no root
  if (discriminant < 0)
  {
    return false;
  }
  // else there is at least one root
  double sqrtd = sqrt(discriminant);

  // find the nearest root that lies in the acceptable range.
  double root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root)
  {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || root > t_max)
    {
      // neither of the roots are acceptable
      return false;
    }
  }

  rec.t = root;
  rec.p = r.point_at_time(rec.t);
  // rec.normal = (rec.p - center) / radius;
  vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr;

  if (mat_ptr->has_texture)
  {
    rec.texture = true;
    vec3 d = (rec.p - center).normalize();
    // cerr << "*" << d << endl;
    rec.u = clamp(0.5 + atan2(d[0], d[2]) / (2 * M_PI), 0, 1) * mat_ptr->texture_w;
    rec.v = clamp(0.5 - asin(d[1]) / (M_PI), 0, 1) * mat_ptr->texture_h;
    // cerr << "**" << rec.u << " " << rec.v << endl;
  }

  return true;
}

#endif