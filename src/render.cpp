#include "all.h"
using namespace std;

const double aspect_ratio = 16.0 / 9.0;
const int image_width = 2048;
const int image_height = static_cast<int>(image_width / aspect_ratio);
const int samples_per_pixel = 50;
const int max_depth = 50;
const color BACKGROUND = color(0.6, 0.6, 0.6);
SCENE scene = GET_SCENE_atmosphere(aspect_ratio);
hittable_list world = scene.world;
bvh_node bbx_root = scene.bbx_root;
camera cam = scene.cam;
// vec3 pixels[image_height][image_width];

color ray_color(ray &r, color background, int depth)
{
  hit_record rec;

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0)
    return color(0, 0, 0);

  // If the ray hits nothing, return the background color.
  if (!bbx_root.hit(r, 0.001, infinity, rec))
    return background;

  ray scattered;
  color attenuation;
  color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

  if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
    return emitted;

  return emitted + attenuation * ray_color(scattered, background, depth - 1);
}
// pixels should be an array of array of color/vec3
// void new_write_color(ostream &out)
// {
//   for (int i = 0; i < image_height; i++)
//   {
//     for (int j = 0; j < image_width; j++)
//     {
//       // v[i][j].join();
//       out << static_cast<int>(255.99 * pixels[i][j].x()) << ' '
//           << static_cast<int>(255.99 * pixels[i][j].y()) << ' '
//           << static_cast<int>(255.99 * pixels[i][j].z()) << '\n';
//     }
//   }
// }

color current_pixel;
void add_color(double u, double v)
{
  ray r = cam.get_ray(u, v);
  current_pixel += ray_color(r, BACKGROUND, max_depth);
}

// color one_pixel_multisample(int i, int j)
// {
//   // cerr << "*" << i << " " << j << endl;
//   current_pixel = color(0, 0, 0);
//   vector<thread> thread_queue;
//   for (int k = 0; k < samples_per_pixel; k++)
//   {
//     double u = (i + rand_unit()) / (image_width - 1);
//     double v = (j + rand_unit()) / (image_height - 1);
//     thread_queue.emplace_back(add_color, u, v);
//   }
//   for (int k = 0; k < samples_per_pixel; k++)
//   {
//     thread_queue[k].join();
//   }
//   return current_pixel;
// }

vec3 pixel[image_width + 1];
void render_pixel(int i, int j)
{
  if (samples_per_pixel == 1)
  {
    double u = (i + 0.5) / (image_width - 1);
    double v = (j + 0.5) / (image_height - 1);
    ray r = cam.get_ray(u, v);
    pixel[i] += ray_color(r, BACKGROUND, max_depth);
    return;
  }

  for (int k = 0; k < samples_per_pixel; k++)
  {
    double u = (i + 0.5) / (image_width - 1);
    double v = (j + 0.5) / (image_height - 1);
    ray r = cam.get_ray(u, v);
    pixel[i] += ray_color(r, BACKGROUND, max_depth);
  }
}
// const int N_THREAD = 1440;
// void render_line(int j)
// {
//   vector<thread> q;

//   for (int i = 0; i < image_width; i += N_THREAD)
//   {
//     for (int num_thread = 0; num_thread < N_THREAD; num_thread++)
//     {
//       if (i + num_thread >= image_width)
//         break;
//       pixel[i + num_thread] = vec3(0, 0, 0);
//       q.emplace_back(render_pixel, i + num_thread, j);
//     }
//     for (int num_thread = 0; num_thread < N_THREAD; num_thread++)
//     {
//       if (i + num_thread >= image_width)
//         break;
//       q[i + num_thread].join();
//     }
//   }
//   for (int i = 0; i < image_width; ++i)
//   {
//     // q[i].join();
//     write_color(cout, pixel[i], samples_per_pixel);
//   }
// }
void render_line(int j)
{
  vector<thread> q;

  for (int i = 0; i < image_width; ++i)
  {
    pixel[i] = vec3(0, 0, 0);
    q.emplace_back(render_pixel, i, j);
  }
  for (int i = 0; i < image_width; ++i)
  {
    q[i].join();
    write_color(cout, pixel[i], samples_per_pixel);
  }
}
int main()
{
  // hit_record rec = hit_record();
  // Material_M material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
  // triangle tri = triangle(point3(1, 1, 1), point3(-1, -1, 1), point3(1, -1, 1), material_right);
  // bool ifhit = tri.hit(ray(vec3(10, 0, 0), vec3(-1, 0, 0)), 0, infinity, rec);
  // cerr << "!!!!" << ifhit << endl;
  auto t1 = chrono::high_resolution_clock::now();
  srand(time(NULL));
  // ignore the return value
  ignore = freopen("render.ppm", "w", stdout);

  // Render
  cout << "P3\n"
       << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j)
  {
    cerr << "\rScanlines remaining: " << j << ' ' << flush;
    render_line(j);
  }
  // new_write_color(cout);
  cerr << "\nDone.\n";
  auto t2 = chrono::high_resolution_clock::now();
  chrono::duration<double, std::milli> ms_double = t2 - t1;
  cerr << ms_double.count() / 1000 << endl;
  return 0;
}