#pragma once
#include <Core/Hittable.h>
#include <Core/Material.h>
#include <Math/AABB.h>

class XYRect : public Hittable
{
public:
   XYRect() = default;
   XYRect(double x0, double x1, double y0, double y1, double k, std::shared_ptr<Material> matPtr) :
      m_x0(x0),
      m_x1(x1),
      m_y0(y0),
      m_y1(y1),
      m_k(k),
      m_matPtr(matPtr)
   {
   }

   bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
   {
      double t = (m_k - r.Origin.z) / r.Direction.z;
      if (t >= tMin && t <= tMax)
      {
         double x = r.Origin.x + (t * r.Direction.x);
         double y = r.Origin.y + (t * r.Direction.y);

         bool bHitXAxis = (x >= m_x0 && x <= m_x1);
         bool bHitYAxis = (y >= m_y0 && y <= m_y1);
         if (bHitXAxis && bHitYAxis)
         {
            rec.u = (x - m_x0) / (m_x1 - m_x0);
            rec.v = (y - m_y0) / (m_y1 - m_y0);
            rec.t = t;

            Vec3 outwardNormal = Vec3(0.0, 0.0, 1.0);
            rec.SetFaceNormal(r, outwardNormal);
            rec.MatPtr = m_matPtr;
            rec.p = r.At(t);
            return true;
         }
      }
      return false;
   }

   bool BoundingBox(double time0, double time1, AABB& outputBox) const override
   {
      constexpr double Padding = 0.0001;
      outputBox = AABB(Point3(m_x0, m_y0, m_k - Padding), Point3(m_x1, m_y1, m_k + Padding));
      return true;
   }

private:
   std::shared_ptr<Material> m_matPtr;
   double m_x0 = -0.5;
   double m_x1 = 0.5;
   double m_y0 = -0.5;
   double m_y1 = 0.5;
   double m_k = 0.0;

};

class XZRect : public Hittable
{
public:
   XZRect() = default;
   XZRect(double x0, double x1, double z0, double z1, double k, std::shared_ptr<Material> matPtr) :
      m_x0(x0),
      m_x1(x1),
      m_z0(z0),
      m_z1(z1),
      m_k(k),
      m_matPtr(matPtr)
   {
   }

   bool Hit(const Ray & r, double tMin, double tMax, HitRecord & rec) const override
   {
      double t = (m_k - r.Origin.y) / r.Direction.y;
      if (t >= tMin && t <= tMax)
      {
         double x = r.Origin.x + (t * r.Direction.x);
         double z = r.Origin.z + (t * r.Direction.z);

         bool bHitXAxis = (x >= m_x0 && x <= m_x1);
         bool bHitZAxis = (z >= m_z0 && z <= m_z1);
         if (bHitXAxis && bHitZAxis)
         {
            rec.u = (x - m_x0) / (m_x1 - m_x0);
            rec.v = (z - m_z0) / (m_z1 - m_z0);
            rec.t = t;

            Vec3 outwardNormal = Vec3(0.0, 1.0, 0.0);
            rec.SetFaceNormal(r, outwardNormal);
            rec.MatPtr = m_matPtr;
            rec.p = r.At(t);
            return true;
         }
      }
      return false;
   }

   bool BoundingBox(double time0, double time1, AABB & outputBox) const override
   {
      constexpr double Padding = 0.0001;
      outputBox = AABB(Point3(m_x0, m_k - Padding, m_z0), Point3(m_x1, m_k + Padding, m_z1));
      return true;
   }

private:
   std::shared_ptr<Material> m_matPtr;
   double m_x0 = -0.5;
   double m_x1 = 0.5;
   double m_z0 = -0.5;
   double m_z1 = 0.5;
   double m_k = 0.0;

};

class YZRect : public Hittable
{
public:
   YZRect() = default;
   YZRect(double y0, double y1, double z0, double z1, double k, std::shared_ptr<Material> matPtr) :
      m_y0(y0),
      m_y1(y1),
      m_z0(z0),
      m_z1(z1),
      m_k(k),
      m_matPtr(matPtr)
   {
   }

   bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
   {
      double t = (m_k - r.Origin.x) / r.Direction.x;
      if (t >= tMin && t <= tMax)
      {
         double y = r.Origin.y + (t * r.Direction.y);
         double z = r.Origin.z + (t * r.Direction.z);

         bool bHitYAxis = (y >= m_y0 && y <= m_y1);
         bool bHitZAxis = (z >= m_z0 && z <= m_z1);
         if (bHitYAxis && bHitZAxis)
         {
            rec.u = (y - m_y0) / (m_y1 - m_y0);
            rec.v = (z - m_z0) / (m_z1 - m_z0);
            rec.t = t;

            Vec3 outwardNormal = Vec3(1.0, 0.0, 0.0);
            rec.SetFaceNormal(r, outwardNormal);
            rec.MatPtr = m_matPtr;
            rec.p = r.At(t);
            return true;
         }
      }
      return false;
   }

   bool BoundingBox(double time0, double time1, AABB& outputBox) const override
   {
      constexpr double Padding = 0.0001;
      outputBox = AABB(Point3(m_k - Padding, m_y0, m_z0), Point3(m_k + Padding, m_y1, m_z1));
      return true;
   }

private:
   std::shared_ptr<Material> m_matPtr;
   double m_y0 = -0.5;
   double m_y1 = 0.5;
   double m_z0 = -0.5;
   double m_z1 = 0.5;
   double m_k = 0.0;

};