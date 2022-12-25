#include <iostream>
#include "vec3.h"
#include <cmath>

using namespace std;

vec3::vec3()
{
  e[0] = 0;
  e[1] = 0;
  e[2] = 0;
}
vec3::vec3(double e0, double e1, double e2)
{
  e[0] = e0;
  e[1] = e1;
  e[2] = e2;
}

double vec3::x() { return e[0]; }
double vec3::y() { return e[1]; }
double vec3::z() { return e[2]; }
double vec3::r() { return e[0]; }
double vec3::g() { return e[1]; }
double vec3::b() { return e[2]; }

vec3 vec3::operator-() { return vec3(-e[0], -e[1], -e[2]); }
// vec3 vec3::operator+() { return *this; }
// vec3 vec3::operator+(vec3 v) { return vec3(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]); }
// vec3 vec3::operator*(double t) { return vec3(t * e[0], t * e[1], t * e[2]); }

// double vec3::operator[](int i) {return e[i];}
double &vec3::operator[](int i) { return e[i]; }

vec3 &vec3::operator+=(vec3 v)
{
  e[0] += v.e[0];
  e[1] += v.e[1];
  e[2] += v.e[2];
  return *this;
}

vec3 &vec3::operator-=(vec3 v)
{
  e[0] -= v.e[0];
  e[1] -= v.e[1];
  e[2] -= v.e[2];
  return *this;
}
vec3 &vec3::operator*=(double t)
{
  e[0] *= t;
  e[1] *= t;
  e[2] *= t;
  return *this;
}

vec3 &vec3::operator/=(double t)
{
  return *this *= 1 / t;
}

double vec3::dot(vec3 v)
{
  return (e[0] * v.e[0] + e[1] * v.e[1] + e[2] * v.e[2]);
}

vec3 &vec3::cross(vec3 v)
{
  *this = vec3(e[1] * v.e[2] - e[2] * v.e[1], e[2] * v.e[0] - e[0] * v.e[2], e[0] * v.e[1] - e[1] * v.e[0]);
  return *this;
}

vec3 &vec3::plus(vec3 &v)
{
  *this = vec3(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]);
  return *this;
}

vec3 &vec3::times(double t)
{
  *this = vec3(e[0] * t, e[1] * t, e[2] * t);
  return *this;
}

double vec3::norm()
{
  return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
}

double vec3::length_sqaure()
{
  return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

vec3 vec3::normalize()
{
  double n = norm();
  return vec3(e[0] / n, e[1] / n, e[2] / n);
}

void vec3::normalized()
{
  double n = norm();
  *this *= 1 / n;
}

bool vec3::near_zero()
{
  double eps = 1e-8;
  return (x() < eps) && (y() < eps) && (z() < eps);
}

ostream &operator<<(std::ostream &out, vec3 v)
{
  return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

istream &operator>>(std::istream &in, vec3 v)
{
  double x1;
  double x2;
  double x3;
  in >> x1 >> x2 >> x3;
  v = vec3(x1, x2, x3);
  return in;
}

vec3 operator+(vec3 u, vec3 v)
{
  return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

vec3 operator-(vec3 u, vec3 v)
{
  return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

vec3 operator*(double t, vec3 v)
{
  return vec3(t * v.x(), t * v.y(), t * v.z());
}

vec3 operator*(vec3 v, double t)
{
  return t * v;
}

vec3 operator/(vec3 v, double t)
{
  return (1 / t) * v;
}

vec3 operator*(vec3 u, vec3 v)
{
  return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

double dot(vec3 u, vec3 v)
{
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

vec3 cross(vec3 u, vec3 v)
{
  return vec3(u.y() * v.z() - u.z() * v.y(),
              u.z() * v.x() - u.x() * v.z(),
              u.x() * v.y() - u.y() * v.x());
}

vec3 refract(vec3 uv, vec3 n, double etai_over_etat)
{
  double cos_theta = fmin(dot(-uv, n), 1.0);
  vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
  vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_sqaure())) * n;
  return r_out_perp + r_out_parallel;
}

vec3 reflect(vec3 v, vec3 n)
{
  return v - 2 * dot(v, n) * n;
}