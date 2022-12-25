#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "../render/material.h"
#include "hittable.h"
#include "hittablelist.h"
#include "vec3.h"
#include "util.h"
#include <cmath>
#include <memory>

const double kEpsilon = 1e-8;
class triangle : public hittable
{
public:
  // vertices
  point3 v1;
  point3 v2;
  point3 v3;
  shared_ptr<material> mat_ptr;
  triangle()
  {
    v1 = vec3();
    v2 = vec3();
    v3 = vec3();
  }
  triangle(point3 a, point3 b, point3 c, shared_ptr<material> m)
  {
    v1 = a;
    v2 = b;
    v3 = c;
    mat_ptr = m;
  }

  bool hit(ray r, double t_min, double t_max, hit_record &rec) override
  {
    vec3 v1v2 = v2 - v1;
    vec3 v1v3 = v3 - v1;
    vec3 pvec = cross(r.velocity, v1v3);
    double det = dot(v1v2, pvec);
    if (det < kEpsilon)
      return false;
    if (std::abs(det) < kEpsilon)
      return false;

    double invDet = 1 / det;

    vec3 tvec = r.origin - v1;
    rec.u = dot(tvec, pvec) * invDet;
    if (rec.u < 0 || rec.u > 1)
      return false;

    vec3 qvec = cross(tvec, v1v2);
    rec.v = dot(r.velocity, qvec) * invDet;
    if (rec.v < 0 || rec.u + rec.v > 1)
      return false;

    rec.t = dot(v1v3, qvec) * invDet;
    rec.p = r.point_at_time(rec.t);
    vec3 n = cross(v1v2, v1v3).normalize();
    if (dot(rec.p - r.origin, n) > 0)
      n = -n;
    rec.normal = n;
    rec.mat_ptr = mat_ptr;
    rec.set_face_normal(r, n);

    // rec.p = p;
    // rec.t = t;

    // return true;
    return true;

    // vec3 a = v1 - v2, b = v1 - v3, c = r.velocity, o = a - r.origin;
    // double deta = det_3(a, b, c);
    // double detx = det_3(o, b, c);
    // double dety = det_3(a, o, c);
    // double detz = det_3(a, b, o);
    // // cerr << v1 << endl
    // //      << v2 << endl
    // //      << v1 - v2 << endl;
    // // cerr << "***" << deta << " " << detx << " " << dety << " " << detz << endl;
    // if (deta == 0)
    //   return false;
    // double beta = detx / deta, gamma = dety / deta, t = detz / deta;
    // if (beta < 0 || gamma < 0 || (beta + gamma) > 1)
    //   return false;
    // if (t < t_min || t > t_max)
    //   return false;
    // point3 p = r.origin + r.velocity * t;
    // vec3 n = cross(a, b).normalize();
    // if (dot(p - r.origin, n) > 0)
    //   n = -n;

    // rec.p = p;
    // rec.t = t;
    // rec.normal = n;
    // rec.mat_ptr = mat_ptr;
    // // rec.set_face_normal(r, n);
    // return true;
  }

  bool bounding_box(aabb &output_box) override
  {
    vec3 min_v =
        vec3(min3(v1[0], v2[0], v3[0]),
             min3(v1[1], v2[1], v3[1]), min3(v1[2], v2[2], v3[2]));
    vec3 max_v =
        vec3(max3(v1[0], v2[0], v3[0]),
             max3(v1[1], v2[1], v3[1]), max3(v1[2], v2[2], v3[2]));
    output_box = aabb(min_v, max_v);
    return true;
  }
};

#endif