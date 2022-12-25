#ifndef LAM_H
#define LAM_H
#include "material.h"
#include "../math/hittable.h"
#include "../math/util.h"
#include "../math/vec3.h"

class lambertian : public material
{
public:
  lambertian(color a) : albedo(a) {}

  // void set_texture(string fileName)
  // {
  //   material::set_texture(fileName);
  // }
  bool scatter(ray &r_in, hit_record &rec, color &attenuation, ray &scattered) override
  {
    vec3 scatter_direction = rec.normal + random_in_unit_sphere();
    // avoid zero sum
    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;

    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    if (has_texture && rec.texture)
    {
      // cerr << rec.u << " " << rec.v << endl;
      attenuation = texture_color[rec.v][rec.u] / 255;
    }
    return true;
  }

public:
  color albedo;
};
using Material_L = shared_ptr<lambertian>;
#endif
