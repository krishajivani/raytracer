#ifndef SCENE_H
#define SCENE_H
#include <iostream>
#include <fstream>
#include <string>
#include "../math/hittablelist.h"
#include "material.h"
#include "metal.h"
#include "lambertian.h"
#include "dielectric.h"
#include "camera.h"
#include "../math/sphere.h"
#include "../math/atmosphere.h"
#include "../math/triangle.h"
#include "../math/bvh_node.h"

struct SCENE
{
  hittable_list world;
  camera cam;
  bvh_node bbx_root;
};
hittable_list mesh_lst = hittable_list();
void obj2lst(string fileName, double downSizeBy = 1.0)
{
  bool flag = true;
  const int MAX = 64000;
  vector<double> vertices[MAX];
  // vector<double> vns[MAX];

  int vacc = 0;
  // int vnacc = 0;
  int facc = 0;
  ifstream myfile;
  Material_M material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
  // freopen(fileName.c_str(), "r", stdin);
  myfile.open(fileName, ios::in);
  std::string mystring;
  while (myfile >> mystring)
  {
    // cerr << mystring << endl;
    if (mystring == "v")
    {
      for (int i = 0; i < 3; i++)
      {
        myfile >> mystring;
        double local = std::stod(mystring) * downSizeBy;
        vertices[vacc].push_back(local);
      }
      vacc++;
    }
    else if (mystring == "f")
    {

      int temp[3];
      for (int i = 0; i < 3; i++)
      {
        myfile >> mystring;
        int endIdx = mystring.rfind('/');
        mystring = mystring.substr(0, endIdx - 1);
        temp[i] = std::stoi(mystring);
      }

      point3 v1 = vec3(vertices[temp[0] - 1][0], vertices[temp[0] - 1][1], vertices[temp[0] - 1][2]);
      point3 v2 = vec3(vertices[temp[1] - 1][0], vertices[temp[1] - 1][1], vertices[temp[1] - 1][2]);
      point3 v3 = vec3(vertices[temp[2] - 1][0], vertices[temp[2] - 1][1], vertices[temp[2] - 1][2]);

      // cerr << temp[0] << " " << temp[1] << " " << temp[2] << endl;
      // cerr << v1.x() << " " << v1.y() << " " << v1.z() << endl;
      // cerr << v2.x() << " " << v2.y() << " " << v2.z() << endl;
      // cerr << v3.x() << " " << v3.y() << " " << v3.z() << endl;
      // flag = false;

      mesh_lst.add(make_shared<triangle>(v1, v2, v3, material_right));
    }
  }
  // myfile.close();
  cerr << "Object Loaded" << endl;
  return;
}
SCENE
GET_SCENE_DEER(double aspect_ratio)
{
  obj2lst("./art/deer.obj", 2);
  Material_L material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  auto LIGHT_Blue = make_shared<light>(color(1, 1, 1));
  mesh_lst.add(make_shared<sphere>(point3(0, 2.7, 4.5), 0.7, LIGHT_Blue));
  // mesh_lst.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
  camera cam(point3(13, 2, 7), point3(0, 0, 0), vec3(0, 1, 0), 50,
             aspect_ratio, 0, 20);

  bvh_node root = bvh_node(mesh_lst.objects, 0, mesh_lst.objects.size(), 0);
  return SCENE{mesh_lst, cam, root};
}
SCENE
GET_SCENE_EARTH(double aspect_ratio)
{
  Material_L material_earth = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  material_earth->set_texture("./art/earthmap4k.ppm");
  mesh_lst.add(make_shared<sphere>(point3(0, 2.7, 4.5), 0.9, material_earth));

  Material_L material_mercury = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  material_mercury->set_texture("./art/2k_mercury.ppm");
  mesh_lst.add(make_shared<sphere>(point3(0, -1, 4.5), 0.9, material_mercury));

  Material_L material_mars = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  material_mars->set_texture("./art/2k_mars.ppm");
  mesh_lst.add(make_shared<sphere>(point3(0, 0.7, 4.5), 0.9, material_mars));

  camera cam(point3(13, 2, 7), point3(0, 0, 0), vec3(0, 1, 0), 50,
             aspect_ratio, 0, 20);

  bvh_node root = bvh_node(mesh_lst.objects, 0, mesh_lst.objects.size(), 0);
  return SCENE{mesh_lst, cam, root};
}
SCENE
GET_SCENE_1(double aspect_ratio)
{
  hittable_list world;
  Material_L material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  Material_L material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  Material_D material_left = make_shared<dielectric>(1.5);
  Material_M material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

  // world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
  // world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
  // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
  // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.45, material_left));
  // world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

  world.add(make_shared<triangle>(point3(0, 0, -1.5), point3(-1, 0, -1), point3(1, 0, -1), material_right));

  camera cam(point3(3, 3, 2), point3(0, 0, -1), vec3(0, 1, 0), 20.0,
             aspect_ratio, 0.1, 10);

  bvh_node root = bvh_node(world.objects, 0, world.objects.size(), 0);
  return SCENE{world, cam, root};
}

SCENE GET_SCENE_random(double aspect_ratio)
{
  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.4, 0.4, 0.4));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  // for (int a = -11; a < 11; a++)
  // {
  //   for (int b = -11; b < 11; b++)
  //   {
  //     auto choose_mat = rand_unit();
  //     point3 center(a + 0.9 * rand_unit(), 0.2, b + 0.9 * rand_unit());

  //     if ((center - point3(4, 0.2, 0)).norm() > 0.9)
  //     {
  //       shared_ptr<material> sphere_material;

  //       if (choose_mat < 0.8)
  //       {
  //         // diffuse
  //         auto albedo = vrandom() * vrandom();
  //         sphere_material = make_shared<lambertian>(albedo);
  //         world.add(make_shared<sphere>(center, 0.2, sphere_material));
  //       }
  //       else if (choose_mat < 0.95)
  //       {
  //         // metal
  //         auto albedo = vrandom();
  //         auto fuzz = rand_range(0, 0.5);
  //         sphere_material = make_shared<metal>(albedo, fuzz);
  //         world.add(make_shared<sphere>(center, 0.2, sphere_material));
  //       }
  //       else
  //       {
  //         // glass
  //         sphere_material = make_shared<dielectric>(1.5);
  //         world.add(make_shared<sphere>(center, 0.2, sphere_material));
  //       }
  //     }
  //   }
  // }

  // glass
  auto material1 = make_shared<dielectric>(1.5);
  // world.add(make_shared<sphere>(point3(0, 1, -2), 1.0, material1));
  //  world.add(make_shared<sphere>(point3(0, 0.5, -2), -0.5, material1));

  auto material2 = make_shared<lambertian>(color(0.8, 0.0, 0.0));
  world.add(make_shared<sphere>(point3(-4, 1, -0.5), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.4, 0.4, 0.7), 0.2);
  world.add(make_shared<sphere>(point3(4, 1, 2), 1.0, material3));

  world.add(make_shared<box>(vec3(6, 0, 3), vec3(7, 1, 4), material2));
  world.add(make_shared<box>(vec3(6, 1, 2.5), vec3(7, 2, 3.5), material1));

  auto LIGHT_Blue = make_shared<light>(color(1, 1, 1));
  auto LIGHT_Orange = make_shared<light>(color(1, 0.5, 0));
  world.add(make_shared<sphere>(point3(4, 0.7, 4.5), 0.7, LIGHT_Blue));
  world.add(make_shared<sphere>(point3(6, 0.3, 0.5), 0.3, LIGHT_Orange));

  world.add(make_shared<box>(vec3(6, 0, -0.5), vec3(7, 1, 0), material1));

  auto material4 = make_shared<metal>(color(0.9, 0.4, 0.5), 0);
  world.add(make_shared<sphere>(point3(6.5, 0.5, 2.5), 0.5, material1));

  auto material5 = make_shared<metal>(color(0.6, 0.2, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, -5), 1.0, material4));

  auto material6 = make_shared<dielectric>(0.7);
  world.add(make_shared<sphere>(point3(8.5, 0.7, 0), 0.7, material6));

  auto material7 = make_shared<metal>(color(0.6, 0.2, 1), 0.0);
  world.add(make_shared<sphere>(point3(-3, 1, -5), 1.0, material7));
  world.add(make_shared<sphere>(point3(-3, 1, 7), 1.0, material7));
  world.add(make_shared<sphere>(point3(-9, 1, -3), 1.0, material7));

  auto LIGHT_Red = make_shared<light>(color(1, 0, 0));
  world.add(make_shared<sphere>(point3(-6.5, 2, 6), 2, LIGHT_Red));

  auto material8 = make_shared<metal>(color(1, 1, 1), 0.2);
  world.add(make_shared<sphere>(point3(-2, 1, 3), 1.0, material8));
  camera cam(point3(13, 2, 4), point3(0, 0, 0), vec3(0, 1, 0), 50,
             aspect_ratio, 0, 20);
  bvh_node root = bvh_node(world.objects, 0, world.objects.size(), 0);
  return SCENE{world, cam, root};
}

SCENE GET_SCENE_atmosphere(double aspect_ratio)
{
  hittable_list world;

  // auto LIGHT_Orange = make_shared<light>(color(1, 0.5, 0));
  // world.add(make_shared<sphere>(point3(0, 0, -13), 1, LIGHT_Orange));

  // Material_L material_mercury = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  // material_mercury->set_texture("./art/2k_mercury.ppm");
  // world.add(make_shared<sphere>(point3(0.3, 4, -13), 0.4, material_mercury));

  // Material_L material_earth = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  // material_earth->set_texture("./art/earthmap4k.ppm");
  // world.add(make_shared<sphere>(point3(-0.3, 7, -13), 0.7, material_earth));

  Material_L material_mars = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  material_mars->set_texture("./art/2k_mars.ppm");
  world.add(make_shared<sphere>(point3(0, 0, 0), 3, material_mars));

  // Material_L material_jupiter = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  // material_jupiter->set_texture("./art/2k_jupiter.ppm");
  // world.add(make_shared<sphere>(point3(-8, 4, -13), 1.2, material_jupiter));
  camera cam(point3(0, 0, 8), point3(0, 0, 0), vec3(0, 1, 0), 50,
             aspect_ratio, 0, 20);
  bvh_node root = bvh_node(world.objects, 0, world.objects.size(), 0);

  return SCENE{world, cam, root};
}
#endif