#ifndef LIGHT_H
#define LIGHT_H
#include "../math/hittablelist.h"
#include "../math/hittable.h"
#include "material.h"
class light : public material
{
public:
  light(shared_ptr<color> a) : emit(a) {}
  light(color c) : emit(make_shared<color>(c)) {}

  virtual bool scatter(
      ray &r_in, hit_record &rec, color &attenuation, ray &scattered) override
  {
    return false;
  }

  virtual color emitted(double u, double v, point3 p) override
  {
    return *emit;
  }

public:
  shared_ptr<color> emit;
};
#endif