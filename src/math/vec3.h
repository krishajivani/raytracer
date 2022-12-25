#ifndef VEC3_H
#define VEC3_H
#include <iostream>
#include <ctime>
class vec3
{
private:
  double e[3];

public:
  vec3();
  vec3(double e0, double e1, double e2);
  // string toString();

  double x();
  double y();
  double z();
  double r();
  double g();
  double b();

  vec3 operator-();
  // vec3 operator+();
  // vec3 operator+(vec3 v);
  // vec3 operator*(double t);
  //   double operator[](int i) { return e[i]; }
  double &operator[](int i);

  // Overload operators for vec3 class

  /* Vector Operations*/
  vec3 &operator+=(vec3 v);

  vec3 &operator-=(vec3 v);

  /* Scalar-Vector Operations*/
  vec3 &operator*=(double t);

  vec3 &operator/=(double t);

  vec3 &cross(vec3 v);

  vec3 &plus(vec3 &v);

  vec3 &times(double t);

  double dot(vec3 v);

  double norm();

  /**Normalize current vector and return it*/
  vec3 normalize();

  /**Normalize current vector*/
  void normalized();

  double length_sqaure();

  bool near_zero();
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

std::ostream &operator<<(std::ostream &out, vec3 v);
std::istream &operator>>(std::istream &in, vec3 v);

vec3 operator+(vec3 u, vec3 v);
vec3 operator-(vec3 u, vec3 v);

vec3 operator*(double t, vec3 v);
vec3 operator*(vec3 v, double t);
vec3 operator*(vec3 u, vec3 v);
vec3 operator/(vec3 v, double t);

double dot(vec3 u, vec3 v);
vec3 cross(vec3 u, vec3 v);

vec3 refract(vec3 uv, vec3 n, double etai_over_etat);
vec3 reflect(vec3 v, vec3 n);
#endif