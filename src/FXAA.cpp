#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
#include <cmath>
#include "math/vec3.h"
using namespace std;
const int MAX_IMG_SIZE = 2800;
const string FILENAME = "render";
const string IN_PATH = FILENAME + ".ppm";
const string OUT_PATH = FILENAME + "_FXAA.ppm";
int img_width;
int img_height;
int max_color;
color img[MAX_IMG_SIZE][MAX_IMG_SIZE];
double luma_img[MAX_IMG_SIZE][MAX_IMG_SIZE];
color new_img[MAX_IMG_SIZE][MAX_IMG_SIZE];
// int clamp(int x, int minx, int maxx)
// {
//   if (x < minx)
//     x = minx;
//   if (x > maxx)
//     x = maxx;
//   return x;
// }
double clamp(double x, double minx, double maxx)
{
  if (x < minx)
    x = minx;
  if (x > maxx)
    x = maxx;
  return x;
}
// bilinear interpolation
double get_interpolate_luma(double x, double y)
{
  int nx = img_width;
  int ny = img_height;
  int fx = (int)(clamp(floor(x), 0, nx - 1));
  int cx = (int)(clamp(floor(x) + 1, 0, nx - 1));
  int fy = (int)(clamp(floor(y), 0, ny - 1));
  int cy = (int)(clamp(floor(y) + 1, 0, ny - 1));
  // cerr << fx << " " << cx << " " << fy << " " << cy << endl;

  double res = luma_img[fy][fx] * (cy - y) * (cx - x) + luma_img[fy][cx] * (x - fx) * (cy - y) +
               luma_img[cy][fx] * (y - fy) * (cx - x) + luma_img[cy][cx] * (x - fx) * (y - fy);
  return res;
}

color get_interpolate_color(double x, double y)
{
  int nx = img_width;
  int ny = img_height;
  int fx = (int)(clamp(floor(x), 0, nx - 1));
  int cx = (int)(clamp(floor(x) + 1, 0, nx - 1));
  int fy = (int)(clamp(floor(y), 0, ny - 1));
  int cy = (int)(clamp(floor(y) + 1, 0, ny - 1));

  color res = img[fy][fx] * (cy - y) * (cx - x) + img[fy][cx] * (x - fx) * (cy - y) +
              img[cy][fx] * (y - fy) * (cx - x) + img[cy][cx] * (x - fx) * (y - fy);
  return res;
}
void write_ppm()
{
  for (int i = 0; i < img_height; i++)
  {
    cerr << "\rScanlines remaining: " << i << ' ' << flush;
    for (int j = 0; j < img_width; j++)
    {
      color p = new_img[i][j];
      int r = static_cast<int>(p.r());
      int g = static_cast<int>(p.g());
      int b = static_cast<int>(p.b());
      printf("%d %d %d\n", r, g, b);
    }
  }
  cerr << endl;
}

double rgb2luma(vec3 c)
{
  return sqrt(c.dot(vec3(0.299, 0.587, 0.114)));
}

int main()
{

  freopen(IN_PATH.c_str(), "r", stdin);
  freopen(OUT_PATH.c_str(), "w", stdout);
  string ppm_format;
  // cerr << IN_PATH.c_str();
  cin >> ppm_format;
  if (ppm_format != "P3")
  {
    cerr << ppm_format << endl;
    cerr << "Invalid Format" << endl;
    return 0;
  }
  cout << ppm_format << endl;

  cin >> img_width >> img_height;
  if (img_width > MAX_IMG_SIZE || img_height > MAX_IMG_SIZE)
  {
    cerr << "Image too large!" << endl;
    return 0;
  }
  cout << img_width << " " << img_height << endl;
  cin >> max_color;
  cout << max_color << endl;

  for (int i = 0; i < img_height; i++)
  {

    for (int j = 0; j < img_width; j++)
    {
      int r, g, b;
      cin >> r >> g >> b;
      img[i][j] = color(r, g, b);
      luma_img[i][j] = rgb2luma(img[i][j]);
    }
  }
  const double EDGE_THRESHOLD_MIN = 0.0312;
  const double EDGE_THRESHOLD_MAX = 0.125;
  const int MAX_ITERATION_NUM_FAXX = 10;
  // cerr << get_rgb(0, 0) << endl;
  // output ppm from img array
  for (int i = 0; i < img_width; i++)
  {
    for (int j = 0; j < img_height; j++)
    {
      double luma_center = luma_img[j][i];
      double luma_down = luma_img[max(j - 1, 0)][i];
      double luma_up = luma_img[min(j + 1, img_height - 1)][i];
      double luma_left = luma_img[j][max(i - 1, 0)];
      double luma_right = luma_img[j][min(i + 1, img_width - 1)];
      double luma_min = min(luma_center, min(min(luma_down, luma_up), min(luma_left, luma_right)));
      double luma_max = max(luma_center, max(max(luma_down, luma_up), max(luma_left, luma_right)));

      if (luma_max - luma_min < max(EDGE_THRESHOLD_MIN, luma_max * EDGE_THRESHOLD_MAX))
      {
        new_img[j][i] = img[j][i];
        continue;
      }

      double luma_downleft = luma_img[max(j - 1, 0)][max(i - 1, 0)];
      double luma_upleft = luma_img[min(j + 1, img_height - 1)][max(i - 1, 0)];
      double luma_downright = luma_img[max(j - 1, 0)][min(i + 1, img_width - 1)];
      double luma_upright = luma_img[min(j + 1, img_height - 1)][min(i + 1, img_width - 1)];

      double edge_horizontal = abs(luma_upleft - luma_left * 2 + luma_downleft) + 2 * abs((luma_up - luma_center) - (luma_center - luma_down)) + abs((luma_upright - luma_right) - (luma_right - luma_downright));
      double edge_vertical = abs((luma_upright - luma_up) - (luma_up - luma_upleft)) + 2 * abs((luma_right - luma_center) - (luma_center - luma_left)) + abs((luma_downright - luma_down) - (luma_down - luma_downleft));
      bool is_hori = (edge_horizontal >= edge_vertical);

      double downside = 0, upside = 0;
      if (is_hori)
      {
        downside = abs(luma_down - luma_center);
        upside = abs(luma_up - luma_center);
      }
      else
      {
        downside = abs(luma_left - luma_center);
        upside = abs(luma_right - luma_center);
      }

      bool is_downside = (downside >= upside);
      double g_scale = 0.25 * max(downside, upside);

      double current_x, current_y, localavg;
      if (is_downside)
      {
        current_x = i - (1 - is_hori) * 0.5;
        current_y = j - is_hori * 0.5;
        localavg = (downside + luma_center) / 2;
      }
      else
      {
        current_x = i + (1 - is_hori) * 0.5;
        current_y = j + is_hori * 0.5;
        localavg = (upside + luma_center) / 2;
      }
      vec3 step_offset;
      if (is_hori)
      {
        step_offset = vec3(1, 0, 0);
      }
      else
      {
        step_offset = vec3(0, 1, 0);
      }
      vec3 uv1 = vec3(current_x, current_y, 0) - step_offset;
      vec3 uv2 = vec3(current_x, current_y, 0) + step_offset;
      bool reached1 = false;
      bool reached2 = false;
      int iter = 1, step_num = 1;
      double luma_end1, luma_end2;
      while ((!reached1 || !reached2) && iter < MAX_ITERATION_NUM_FAXX)
      {
        // step num = 1 1 2 2 4 8 16 32 64 128 ...
        if (iter <= 2)
        {
          step_num = 1;
        }
        else if (iter <= 4)
          step_num = 2;
        else
        {
          step_num = pow(2, (iter - 3));
        }
        if (!reached1)
        {
          luma_end1 = get_interpolate_luma(uv1[0], uv1[1]) - localavg;
        }
        if (!reached2)
        {
          luma_end2 = get_interpolate_luma(uv2[0], uv2[1]) - localavg;
        }
        reached1 = (abs(luma_end1) >= g_scale);
        reached2 = (abs(luma_end2) >= g_scale);
        if (!reached1)
        {
          uv1 += step_offset * step_num;
        }
        if (!reached1)
        {
          uv1 += step_offset * step_num;
        }
        iter++;
      }
      double dist1, dist2;
      if (is_hori)
      {
        dist1 = abs(uv1[0] - current_x);
        dist2 = abs(uv2[0] - current_x);
      }
      else
      {
        dist1 = abs(uv1[1] - current_y);
        dist2 = abs(uv2[1] - current_y);
      }
      bool is_dir1 = (dist1 <= dist2);
      double edge_len = dist1 + dist2 + 1e-6;
      double pixel_offset = -min(dist1, dist2) / edge_len + 0.5;
      double luma_closer = 0;
      if (is_dir1)
      {
        luma_closer = luma_end1;
      }
      else
      {
        luma_closer = luma_end2;
      }
      if ((luma_center < localavg and luma_closer < 0) or (luma_center > localavg and luma_closer > 0))
        pixel_offset = 0;
      double subpixel_avg = 2 * (luma_down + luma_up + luma_left + luma_right) +
                            (luma_downright + luma_downleft + luma_upright + luma_upleft);
      subpixel_avg /= 12.0;

      double sub_tmp1 = clamp(abs(subpixel_avg - luma_center) / (luma_max - luma_min), 0.0, 1.0);
      double sub_tmp2 = (-2.0 * sub_tmp1 + 3.0) * sub_tmp1 * sub_tmp1;
      double sub_final = 0.75 * sub_tmp2 * sub_tmp2;
      pixel_offset = max(pixel_offset, sub_final);
      new_img[j][i] = get_interpolate_color(i + (1 - is_hori) * pixel_offset, j + is_hori * pixel_offset);
    }
  }

  write_ppm();
  return 0;
}