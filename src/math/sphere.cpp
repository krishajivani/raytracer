// DEPRECATED
// NOW IN SHPERE.H FILE

// #include <memory>
// #include "sphere.h"
// #include "vec3.h"
// #include <cmath>

// sphere::sphere()
// {
//   center = vec3();
//   radius = 0;
// }

// sphere::sphere(point3 cen, double rad, shared_ptr<material> m)
// {
//   center = cen;
//   radius = rad;
//   mat_ptr = m;
// }

// bool sphere::hit(ray r, double t_min, double t_max, hit_record &rec)
// {
//   vec3 oc = r.origin - center;
//   double a = pow(r.velocity.norm(), 2);
//   double half_b = dot(oc, r.velocity);
//   double c = pow(oc.norm(), 2) - pow(radius, 2);

//   double discriminant = half_b * half_b - a * c;
//   // if there is no root
//   if (discriminant < 0)
//   {
//     return false;
//   }
//   // else there is at least one root
//   double sqrtd = sqrt(discriminant);

//   // find the nearest root that lies in the acceptable range.
//   double root = (-half_b - sqrtd) / a;
//   if (root < t_min || t_max < root)
//   {
//     root = (-half_b + sqrtd) / a;
//     if (root < t_min || root > t_max)
//     {
//       // neither of the roots are acceptable
//       return false;
//     }
//   }

//   rec.t = root;
//   rec.p = r.point_at_time(rec.t);
//   // rec.normal = (rec.p - center) / radius;
//   vec3 outward_normal = (rec.p - center) / radius;
//   rec.set_face_normal(r, outward_normal);
//   rec.mat_ptr = mat_ptr;

//   return true;
// }