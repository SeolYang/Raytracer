#pragma once
#include <Math/MathMinimal.h>
#include <iostream>

class Vec3
{
public:
   Vec3() : Vec3(0.0, 0.0, 0.0)
   {
   }

   Vec3(double xx, double yy, double zz) :
      x(xx),
      y(yy),
      z(zz)
   {
   }

   inline const Vec3& operator+() const { return *this; }
   inline Vec3 operator-() const { return Vec3(-x, -y, -z); }
   inline double operator[](int idx) const { return e[idx]; }
   inline double& operator[](int idx) { return e[idx]; }

   inline Vec3& operator+=(const Vec3& v2);
   inline Vec3& operator-=(const Vec3& v2);
   inline Vec3& operator*=(const Vec3& v2);
   inline Vec3& operator/=(const Vec3& v2);
   inline Vec3& operator*=(const double t);
   inline Vec3& operator/=(const double t);

   inline double Length() const {
      return std::sqrt(x * x + y * y + z * z);
   }

   inline double SquaredLength() const {
      return (x * x + y * y + z * z);
   }

   inline void MakeAsUnit();

   inline static Vec3 Random()
   {
      return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
   }

   inline static Vec3 Random(double min, double max)
   {
      return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
   }

   inline bool IsNearZero() const
   {
      const auto eps = 1e-8;
      return (fabs(x) < eps) && (fabs(y) < eps) && (fabs(z) < eps);
   }

public:
   union
   {
      double e[3];
      struct
      {
         double x;
         double y;
         double z;
      };

      struct
      {
         double r;
         double g;
         double b;
      };
   };
};

using Point3 = Vec3;

inline std::istream& operator>>(std::istream& is, Vec3& t)
{
   is >> t.x >> t.y >> t.z;
   return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& t)
{
   os << t.x << " " << t.y << " " << t.z;
   return os;
}

inline void Vec3::MakeAsUnit()
{
   double k = 1.0 / sqrt(x * x + y * y + z * z);
   x *= k;
   y *= k;
   z *= k;
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
   return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
   return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
   return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2)
{
   return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

inline Vec3 operator*(double t, const Vec3& v)
{
   return Vec3(t * v.x, t * v.y, t * v.z);
}

inline Vec3 operator*(const Vec3& v, double t)
{
   return Vec3(t * v.x, t * v.y, t * v.z);
}

inline Vec3 operator/(const Vec3& v, double t)
{
   return Vec3(v.x / t, v.y / t, v.z / t);
}

inline double Dot(const Vec3& v1, const Vec3& v2)
{
   return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline Vec3 Cross(const Vec3& v1, const Vec3& v2)
{
   return Vec3(
      (v1.y * v2.z - v1.z * v2.y),
      (-(v1.x * v2.z - v1.z * v2.x)),
      (v1.x * v2.y - v1.y * v2.x)
   );
}

inline Vec3& Vec3::operator+=(const Vec3& v)
{
   x += v.x;
   y += v.y;
   z += v.z;
   return (*this);
}

inline Vec3& Vec3::operator-=(const Vec3& v)
{
   x -= v.x;
   y -= v.y;
   z -= v.z;
   return (*this);
}

inline Vec3& Vec3::operator*=(const Vec3& v)
{
   x *= v.x;
   y *= v.y;
   z *= v.z;
   return (*this);
}

inline Vec3& Vec3::operator/=(const Vec3& v)
{
   x /= v.x;
   y /= v.y;
   z /= v.z;
   return (*this);
}

inline Vec3& Vec3::operator*=(const double t)
{
   x *= t;
   y *= t;
   z *= t;
   return (*this);
}

inline Vec3& Vec3::operator/=(const double t)
{
   double k = 1.0 / t;
   x *= k;
   y *= k;
   z *= k;
   return (*this);
}

inline Vec3 UnitVectorOf(const Vec3& v)
{
   return (v / v.Length());
}

inline Vec3 RandomInUnitSphere()
{
   while (true)
   {
      auto p = Vec3::Random(-1.0, 1.0);
      if (p.SquaredLength() >= 1.0)
      {
         continue;
      }

      return p;
   }
}

inline Vec3 RandomUnitVector()
{
   return UnitVectorOf(RandomInUnitSphere());
}

inline Vec3 RandomInHemisphere(const Vec3& normal)
{
   Vec3 inUnitSphere = RandomInUnitSphere();
   if (Dot(inUnitSphere, normal) > 0.0) // Normal과 같은 Hemisphere 상에 있음
   {
      return inUnitSphere;
   }
   else // Normal의 반대 Hemisphere에 있음
   {
      return -inUnitSphere;
   }
}

inline Vec3 RandomInUnitDisk()
{
   while (true)
   {
      auto p = Vec3(RandomDouble(-1.0, 1.0), RandomDouble(-1.0, 1.0), 0.0);
      if (p.SquaredLength() >= 1.0)
      {
         continue;
      }

      return p;
   }
}

inline Vec3 Reflect(const Vec3& v, const Vec3& n)
{
   return v - 2.0 * Dot(v, n) * n;
}

inline Vec3 Refract(const Vec3& inDir, const Vec3& n, double ior)
{
   auto cosTheta = fmin(Dot(-inDir, n), 1.0);
   Vec3 rayOutPerp = ior * (inDir + cosTheta * n);
   Vec3 rayOutParallel = -std::sqrt(std::fabs(1.0 - rayOutPerp.SquaredLength())) * n;
   return (rayOutPerp + rayOutParallel);
}