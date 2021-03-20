#pragma once
#include <Core/CoreMinimal.h>
#include <Math/Ray.h>
#include <Math/AABB.h>

struct HitRecord
{
public:
   inline void SetFaceNormal(const Ray& r, const Vec3& outwardNormal)
   {
      bFrontFace = Dot(r.Direction, outwardNormal) < 0;
      n = bFrontFace ? outwardNormal : -outwardNormal;
   }

public:
   Point3 p;
   Vec3 n;
   std::shared_ptr<class Material> MatPtr;
   double t = 0.0;
   double u = 0.0;
   double v = 0.0;
   bool bFrontFace = false;

};

class Hittable
{
public:
   virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
   virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const = 0;

};