#ifndef BOX_H
#define BOX_H
#include "rect.h"
#include "hittablelist.h"
// #include "aabb.h"
#include <memory>

class box : public hittable
{
public:
  box() {}
  box(point3 p0, point3 p1, shared_ptr<material> ptr);

  virtual bool hit(ray r, double t_min, double t_max, hit_record &rec) override;
  virtual bool bounding_box(aabb &output_box) override;

public:
  point3 box_min;
  point3 box_max;
  hittable_list sides;
};

box::box(point3 p0, point3 p1, shared_ptr<material> ptr)
{
  box_min = p0;
  box_max = p1;

  sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
  sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

  sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
  sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

  sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
  sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool box::hit(ray r, double t_min, double t_max, hit_record &rec)
{
  return sides.hit(r, t_min, t_max, rec);
}

bool box::bounding_box(aabb &output_box)
{
  output_box = aabb(box_min, box_max);
  return true;
}

#endif