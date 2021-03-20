#pragma once
#include <Core/Hittable.h>

class Sphere : public Hittable
{
public:
   Sphere(Point3 center, double radius, std::shared_ptr<Material> materialPtr) :
      Center(center),
      Radius(radius),
      MaterialPtr(materialPtr)
   {
   }

   bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override 
   {
      Vec3 centerToOrigin = r.Origin - Center;
      auto a = r.Direction.SquaredLength();
      auto halfB = Dot(centerToOrigin, r.Direction);
      auto c = centerToOrigin.SquaredLength() - Radius * Radius;

      auto discriminant = halfB * halfB - a * c;
      if (discriminant < 0.0)
      {
         return false;
      }
      auto sqrtDiscriminant = sqrt(discriminant);

      auto root = (-halfB - sqrtDiscriminant) / a;
      if (root < tMin || tMax < root)
      {
         root = (-halfB + sqrtDiscriminant) / a;
         if (root < tMin || tMax < root)
         {
            return false;
         }
      }

      rec.t = root;
      rec.p = r.At(rec.t);
      Vec3 outwardNormal = (rec.p - Center) / Radius;
      rec.SetFaceNormal(r, outwardNormal);
      Sphere::GetSphereUV(outwardNormal, rec.u, rec.v);
      rec.MatPtr = MaterialPtr;

      return true;
   }

   bool BoundingBox(double time0, double time1, AABB& outputBox) const override
   {
      outputBox = AABB(
         Center - Vec3(Radius, Radius, Radius),
         Center + Vec3(Radius, Radius, Radius));
      return true;
   }

   static void GetSphereUV(const Point3& p, double& u, double& v)
   {
      auto theta = std::acos(-p.y);
      auto phi = std::atan2(-p.z, p.x) + Pi;

      u = phi / (2.0 * Pi);
      v = theta / Pi;
   }

public:
   Point3 Center = Point3();
   double Radius = 1.0;
   std::shared_ptr<class Material> MaterialPtr;

};