#include "hittable.h"
#include "hittablelist.h"
#include <cmath>
#include <memory>
#include <vector>
// #include "aabb.h"

using namespace std;

hittable_list::hittable_list() {}

void hittable_list::add(shared_ptr<hittable> object)
{
  objects.push_back(object);
}

hittable_list::hittable_list(shared_ptr<hittable> object)
{
  add(object);
}
void hittable_list::clear()
{
  objects.clear();
}

// bool hittable_list::hit(ray r, double t_min, double t_max, hit_record &rec)
// {
//   hit_record temp_rec;
//   bool hit_anything = false;
//   double closest_so_far = t_max;

//   for (int i = 0; i < objects.size(); i++)
//   {
//     shared_ptr<hittable> object = objects[i];
//     if (object->hit(r, t_min, closest_so_far, temp_rec))
//     {
//       hit_anything = true;
//       closest_so_far = temp_rec.t;
//       rec = temp_rec;
//     }
//   }

//   return hit_anything;
// }

bool hittable_list::hit(ray r, double t_min, double t_max, hit_record &rec)
{
  hit_record temp_rec;
  bool hit_anything = false;
  double closest_so_far = t_max;

  for (int i = 0; i < objects.size(); i++)
  {
    shared_ptr<hittable> object = objects[i];
    if (object->hit(r, t_min, closest_so_far, temp_rec))
    {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

bool bbox_helper(aabb &bbox1, aabb &bbox2)
{
  point3 v1 = vec3(std::min(bbox1.minimum.x(), bbox2.minimum.x()), std::min(bbox1.minimum.y(), bbox2.minimum.y()), std::min(bbox1.minimum.z(), bbox2.minimum.z()));
  point3 v2 = vec3(std::max(bbox1.maximum.x(), bbox2.maximum.x()), std::max(bbox1.maximum.y(), bbox2.maximum.y()), std::max(bbox1.maximum.z(), bbox2.maximum.z()));

  bbox1 = aabb(v1, v2);
  return true;
}

bool hittable_list::bounding_box(aabb &output_box)
{
  if (objects.empty())
    return false;
  else
  {
    aabb temp = aabb();
    objects[0]->bounding_box(temp);
    for (int i = 1; i < objects.size(); i++)
    {
      aabb local = aabb();
      if (!objects[i]->bounding_box(local))
        return false;
      bbox_helper(temp, local);
    }
    return true;
  }
}