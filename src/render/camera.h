#ifndef CAMERA_H
#define CAMERA_H

#include "../math/vec3.h"
#include "../math/ray.h"
#include "../math/util.h"

class camera
{
private:
  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  double lens_radius;

public:
  camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov,
         double aspect_ratio, double aperture, double focus_dist)
  {
    // double aperture = 2.0;
    // double focus_dist = (lookfrom - lookat).norm();

    double theta = degrees_to_radians(vfov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    w = (lookfrom - lookat).normalize();
    u = cross(vup, w).normalize();
    v = cross(w, u);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

    lens_radius = aperture / 2;

    // double aspect_ratio = 16.0 / 9.0;
    // double viewport_height = 2.0;
    // double viewport_width = aspect_ratio * viewport_height;
    // double focal_length = 1.0;

    // origin = point3(0, 0, 0);
    // horizontal = vec3(viewport_width, 0.0, 0.0);
    // vertical = vec3(0.0, viewport_height, 0.0);
    // lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
  }

  ray get_ray(double u1, double v1)
  {
    // return ray(origin, lower_left_corner + u1 * horizontal + v1 * vertical - origin);

    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();

    return ray(
        origin + offset,
        lower_left_corner + u1 * horizontal + v1 * vertical - origin - offset);
  }
};

#endif