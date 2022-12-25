#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <cmath>
#include <memory>
#include <vector>
// #include "aabb.h"

// shared_ptr is a smart pointer that retains
// shared ownership of an object through a pointer
using std::shared_ptr;
// this returns a shared_ptr
using std::make_shared;
using namespace std;
class hittable_list : public hittable
{
public:
  vector<shared_ptr<hittable>> objects;
  hittable_list();
  hittable_list(shared_ptr<hittable> object);
  void clear();
  void add(shared_ptr<hittable> object);
  virtual bool hit(ray r, double t_min, double t_max, hit_record &rec) override;
  virtual bool bounding_box(aabb &output_box) override;
};

#endif