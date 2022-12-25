#include <iostream>
#include "math/vec3.h"
#include "math/ray.h"
using namespace std;

vec3 ray_color(ray r)
{
  vec3 unit_direction = r.velocity.normalize();
  double t = 0.5 * (unit_direction.y() + 1.0);
  vec3 res = vec3(1.0, 1.0, 1.0) * (1.0 - t) + vec3(0.5, 0.7, 1.0) * t;
  return res;
}

int main()
{
  point3 a = vec3(1, 2, 3);
  point3 b = vec3(1, 2, 3);
  // point3 b = vec3(3, 1, 3);
  // point3 tmp = a + b;
  a += b;
  a += b;
  cout << a << endl;
  cout << dot(a, b) << endl;
  cout << a << endl;
  a -= b;
  cout << a.cross(b) << endl;

  ray xaxis = ray(vec3(0, 0, 0), vec3(1, 0, 0));

  vec3 direct = xaxis.velocity;

  cout << direct << endl;

  point3 x10 = xaxis.point_at_time(10);

  cout << x10 << endl;

  // freopen("1.ppm", "w", stdout);
  // //  Image
  // const int image_width = 768;
  // const int image_height = 768;
  // // Render
  // cout << "P3 \n"
  //      << image_width << " " << image_height << " " << '\n';
  // cout << "255 " << '\n';
  // for (int j = image_height - 1; j >= 0; --j)
  // {
  //   // std::cerr << "\rScanlines remaining: " << j << ' ' << flush;
  //   for (int i = 0; i < image_width; ++i)
  //   {
  //     double r = double(i) / (image_width - 1);
  //     double g = double(j) / (image_height - 1);
  //     double b = 0.25;

  //     int ir = static_cast<int>(255.999 * r);
  //     int ig = static_cast<int>(255.999 * g);
  //     int ib = static_cast<int>(255.999 * b);

  //     cout << ir << ' ' << ig << ' ' << ib << ' ' << '\n';
  //   }
  // }

  // vec3 lower_left_corner(-2.0, -1.0, -1.0);
  // vec3 horizontal(4.0, 0.0, 0.0);
  // vec3 vertical(0.0, 2.0, 0.0);
  // vec3 origin(0.0, 0.0, 0.0);
  // for (int j = image_height - 1; j >= 0; j--)
  // {
  //   for (int i = 0; i < image_width; i++)
  //   {
  //     float u = float(i) / float(image_width);
  //     float v = float(j) / float(image_height);
  //     ray r = ray(origin, (lower_left_corner + horizontal * u + vertical * v));
  //     vec3 col = ray_color(r);
  //     int ir = int(255.99 * col[0]);
  //     int ig = int(255.99 * col[1]);
  //     int ib = int(255.99 * col[2]);

  //     std::cout << ir << ' ' << ig << ' ' << ib << ' ' << "\n";
  //   }
  // }
  // std::cerr << "\nDone.\n";
  return 0;
}