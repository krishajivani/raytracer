#ifndef MATERIAL_H
#define MATERIAL_H
#include <memory>
#include "../math/ray.h"
#include <fstream>
#include <string>
using namespace std;
struct hit_record;

class material
{
public:
  bool diffuse = false;
  bool metal = false;
  bool has_texture = false;
  color texture_color[5000][5000];
  int texture_w, texture_h;

  void set_texture(string fileName)
  {
    has_texture = true;
    std::string ppm_format;
    ifstream myfile;
    myfile.open(fileName, ios::in);
    myfile >> ppm_format;
    if (ppm_format != "P3")
    {
      cerr << ppm_format << std::endl;
      cerr << "Invalid Format" << std::endl;
      return;
    }
    myfile >> texture_w >> texture_h;
    if (texture_w > 5000 || texture_h > 5000)
    {
      cerr << "Image too large!" << std::endl;
      return;
    }
    int max_color;
    myfile >> max_color;

    for (int i = 0; i < texture_h; i++)
    {
      for (int j = 0; j < texture_w; j++)
      {
        int r, g, b;
        myfile >> r >> g >> b;
        texture_color[i][j] = color(r, g, b);
      }
    }
    myfile.close();
  }

  virtual bool scatter(ray &r_in, hit_record &rec,
                       color &attenuation, ray &scattered) = 0;
  virtual color emitted(double u, double v, point3 p)
  {
    return color(0, 0, 0);
  }
};

#endif