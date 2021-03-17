#pragma once
#include <Core/CoreMinimal.h>
#include <Core/Hittable.h>
#include <Math/Ray.h>

class Material;
class MovingSphere : public Hittable
{
public:
   MovingSphere() = default;
   MovingSphere(Point3 center0, Point3 center1, double time0, double time1, double rad, std::shared_ptr<Material> matPtr) :
      Center0(center0), Center1(center1),
      Time0(time0), Time1(time1),
      Radius(rad),
      MatPtr(matPtr)
   {
   }

   bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
   {
      Vec3 centerToOrigin = r.Origin - this->Center(r.Time);
      auto a = r.Direction.SquaredLength();
      auto halfB = Dot(centerToOrigin, r.Direction);
      auto c = centerToOrigin.SquaredLength() - Radius * Radius;

      auto discriminant = halfB * halfB - a * c;
      if (discriminant < 0.0)
      {
         // no root
         return false;
      }

      auto sqrtDiscriminant = std::sqrt(discriminant);
      auto root = (-halfB - sqrtDiscriminant) / a;
      if (root < tMin || root > tMax)
      {
         root = (-halfB + sqrtDiscriminant) / a;
         if (root < tMin || root > tMax)
         {
            return false;
         }
      }

      rec.t = root;
      rec.p = r.At(rec.t);
      auto outwardNormal = (rec.p - Center(r.Time)) / Radius;
      rec.SetFaceNormal(r, outwardNormal);
      rec.MatPtr = MatPtr;
      return true;
   }

   Point3 Center(double currentTime) const { return Center0 + (((currentTime - Time0) / (Time1 - Time0)) * (Center1 - Center0)); }

public:
   Point3 Center0, Center1;
   double Time0 = 0.0, Time1 = 0.0;
   double Radius = 1.0;
   std::shared_ptr<Material> MatPtr;

};