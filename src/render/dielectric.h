#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"
#include "../math/hittable.h"
#include "../math/util.h"
#include "../math/vec3.h"

class dielectric : public material
{

private:
  static double reflectance(double cosine, double ref_idx)
  {
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }

public:
  double ir; //reflection index

  dielectric(double index_of_refraction) : ir(index_of_refraction) {}

  virtual bool scatter(
      ray &r_in, hit_record &rec, color &attenuation, ray &scattered) override
  {
    attenuation = color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

    vec3 unit_direction = r_in.velocity.normalize();
    // vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot = refraction_ratio * sin_theta > 1.0;
    vec3 direction;

    if (cannot || reflectance(cos_theta, refraction_ratio) > rand_unit())
    {
      direction = reflect(unit_direction, rec.normal);
    }
    else
    {
      direction = refract(unit_direction, rec.normal, refraction_ratio);
    }
    scattered = ray(rec.p, direction);
    return true;
  }
};
using Material_D = shared_ptr<dielectric>;
#endif