#ifndef METAL_H
#define METAL_H
#include "material.h"
#include "../math/hittable.h"
#include "../math/util.h"
#include "../math/vec3.h"

class metal : public material
{
public:
  metal(const color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1.0) {}

  bool scatter(
      ray &r_in, hit_record &rec, color &attenuation, ray &scattered) override
  {
    vec3 unit_ray = r_in.velocity.normalize();
    vec3 reflected = unit_ray - 2 * dot(unit_ray, rec.normal) * rec.normal;
    scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    return (dot(scattered.velocity, rec.normal) > 0);
  }

public:
  color albedo;
  double fuzz;
};
using Material_M = shared_ptr<metal>;
#endif