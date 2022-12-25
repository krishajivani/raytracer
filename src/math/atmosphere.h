#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include "../render/material.h"
#include "hittable.h"
#include "sphere.h"
#include "hittablelist.h"
// #include "aabb.h"
#include "vec3.h"
#include <cmath>
#include <memory>

class atmosphere : public hittable
{
public:
  //settings
  int inScatteringPoints = 10;
  //optical depth is the density of the atmosphere,
  //it determines how much or how likely the light is bouced off the "ray"
  //higher optical depth, lower transmittance, transmisttance = e^-depth
  //again we use 10 samples points, which shoudl be good enough for a large scale picture
  int opticalDepthPoints = 10;
  //parameter that controls how quickly the atmosphere density changes
  double densityFalloff = 4;
  double atomsphreScale = 1; //depth of the atmosphere
  vec3 wavelengths = vec3(700, 530, 440);
  double scatteringStrength = 6.2;
  double scatterR = std::pow(400 / wavelengths.x(), 4) * scatteringStrength;
  double scatterG = std::pow(400 / wavelengths.y(), 4) * scatteringStrength;
  double scatterB = std::pow(400 / wavelengths.z(), 4) * scatteringStrength;
  vec3 coeffs = vec3(scatterR, scatterG, scatterB);
  //hittable usuals
  std::shared_ptr<sphere> planet;
  double planetRadius;
  point3 center;
  double radius;

  shared_ptr<material> mat_ptr;

  // aabb bbox;
  atmosphere();
  atmosphere(std::shared_ptr<sphere> plt, point3 center, double rad, shared_ptr<material> m);
  virtual bool hit(ray r, double t_min, double t_max, hit_record &rec) override;
  virtual bool bounding_box(aabb &output_box) override;

  // dist should return (dist1, dist2, _)
  // dist1 is the distance between the hit point of the ray on the atmosphere
  // dist2 is the distance between the hit point of the ray on either the planet
  // of the other side of the sphere.
  // what if the ray starts inside the atmosphere
  // the distance should be used to determine the intensity of in-scattering light
  // as different lights have different wavelength, the intensity of different
  // light received shoudl be differen

  vec3 dist(ray r);

  // for the calculation of light, we need a couple of helper functions
  // 1. how much light get to the in scattering point (This times the transmittance)
  // 2. at the in sacttering point, how much of it will go in the desired direction
  // the peanut function with angles
  // 3. again, on its way to human eye, it can also be bounced off to somewhere else
  // use for loop to calculate the sum of the light at each in scattering point

  double densityAtPoint(vec3 densitySamplePoint);
  double opticalDepth(ray r, double ray_length);
  vec3 calculateLight(ray r, double ray_length, double t_min, double t_max, hit_record &rec);
};

atmosphere::atmosphere()
{
  planet = NULL;
  planetRadius = 0;
  center = vec3();
  radius = 0;
}

atmosphere::atmosphere(std::shared_ptr<sphere> plt, point3 cen, double rad, shared_ptr<material> m)
{
  planet = plt;
  center = cen;
  radius = rad;
  planetRadius = plt->radius;
  mat_ptr = m;
}

bool atmosphere::bounding_box(aabb &output_box)
{
  point3 v1 = vec3(center.x() - radius,
                   center.y() - radius, center.z() - radius);
  point3 v2 = vec3(center.x() + radius,
                   center.y() + radius, center.z() + radius);
  output_box = aabb(v1, v2);
  return true;
}

bool atmosphere::hit(ray r, double t_min, double t_max, hit_record &rec)
{
  if (!planet->hit(r, t_min, t_max, rec))
  {

    vec3 oc = r.origin - center;
    double a = r.velocity.length_sqaure();
    double half_b = dot(oc, r.velocity);
    double c = oc.length_sqaure() - radius * radius;

    double discriminant = half_b * half_b - a * c;
    // if there is no root
    if (discriminant < 0)
    {
      return false;
    }
    // else there is at least one root
    double sqrtd = sqrt(discriminant);

    // find the nearest root that lies in the acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
      root = (-half_b + sqrtd) / a;
      if (root < t_min || root > t_max)
      {
        // neither of the roots are acceptable
        return false;
      }
    }

    rec.t = root;
    rec.p = r.point_at_time(rec.t);
    // rec.normal = (rec.p - center) / radius;
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
  }

  return planet->hit(r, t_min, t_max, rec);
}

vec3 atmosphere::dist(ray r)
{
  vec3 oc = r.origin - center;
  double a = 1;
  double b = 2 * dot(oc, r.velocity);
  double c = dot(oc, oc) - radius * radius;
  double d = b * b - 4 * a * c;

  if (d > 0)
  {
    double s = sqrt(d);
    double dstToNear = std::max(0.0, (-b - s) / (2 * a));
    double dstToFar = (-b + s) / (2 * a);
    if (dstToFar >= 0)
    {
      return vec3(dstToNear, dstToFar - dstToNear, 0);
    }
  }
  return vec3(1000, 0, 0);
}
// first helper, point density, exponentially thiner air as you go up

double atmosphere::densityAtPoint(vec3 densitySamplePoint)
{
  double heightAboveSurface = (densitySamplePoint - center).norm() - planetRadius;
  double height01 = heightAboveSurface / (radius - planetRadius);
  double localDensity = exp(-height01 * densityFalloff) * (1 - height01);
  return localDensity;
}

// second helper, calculate the accumulated density along a ray/path
double atmosphere::opticalDepth(ray r, double ray_length)
{
  //this ray is the ray that is coming into the in sacttering point
  //I think it is a valid assumption to say that the sun is super super far
  //away, the ray start from the top surface of the atmosphere
  vec3 densitySamplePoint = r.origin;
  double stepSize = ray_length / (opticalDepthPoints - 1);
  double opticalDepth = 0;
  for (int i = 0; i < opticalDepthPoints; i++)
  {
    double localDensity = densityAtPoint(densitySamplePoint);
    opticalDepth += localDensity * stepSize;
    densitySamplePoint += r.velocity * stepSize;
  }
  return opticalDepth;
}

vec3 atmosphere::calculateLight(ray r, double ray_length, double t_min, double t_max, hit_record &rec)
{
  //again, this ray is not a fake camera ray, instead, it is the
  vec3 inScatteringPoint = r.origin;
  double stepSize = ray_length / (inScatteringPoints - 1);
  double inScatteringLight = 0;

  for (int i = 0; i < inScatteringPoints; i++)
  {
    // double sunRayLength = dist(r).y();
    // double sunRayOpticalDepth = opticalDepth(inScatteringPoint,10, sunRayLength);
    // double viewRayOpticalDepth = opticalDepth(inScatteringPoint, -r.velocity, stepSize*i);
  }
  return vec3();
}

#endif