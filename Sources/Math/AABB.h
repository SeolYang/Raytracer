#pragma once
#include <Math/Ray.h>

class AABB
{
public:
   AABB() = default;
   AABB(const Point3& min, const Point3& max) :
      Minimum(min),
      Maximum(max)
   {
   }

   bool Hit(const Ray& r, double tMin, double tMax) const
   {
      for (size_t dim = 0; dim < 3; ++dim)
      {
         auto invD = 1.0 / r.Direction[dim];
         auto t0 = (Minimum[dim] - r.Origin[dim]) * invD;
         auto t1 = (Maximum[dim] - r.Origin[dim]) * invD;

         if (invD < 0.0)
         {
            std::swap(t0, t1);
         }

         tMin = t0 > tMin ? t0 : tMin;
         tMax = t1 < tMax ? t1 : tMax;
         if (tMax <= tMin)
         {
            return false;
         }
      }

      return true;
   }

   static AABB SurroundingBox(const AABB& box0, const AABB& box1)
   {
      Point3 min(
         std::fmin(box0.Minimum.x, box1.Minimum.x),
         std::fmin(box0.Minimum.y, box1.Minimum.y),
         std::fmin(box0.Minimum.z, box1.Minimum.z));

      Point3 max(
         std::fmax(box0.Maximum.x, box1.Maximum.x),
         std::fmax(box0.Maximum.y, box1.Maximum.y),
         std::fmax(box0.Maximum.z, box1.Maximum.z));

      return AABB(min, max);
   }

public:
   Point3 Minimum;
   Point3 Maximum;

};