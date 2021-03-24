#pragma once
#include <Core/Hittable.h>

class Translate : public Hittable
{
public:
   Translate(std::shared_ptr<Hittable> src, const Vec3& displacement) :
      m_src(src),
      m_displacement(displacement)
   {
   }

   bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
   {
      Ray movedRay = Ray(r.Origin - m_displacement, r.Direction, r.Time);
      if (!m_src->Hit(movedRay, tMin, tMax, rec))
      {
         return false;
      }

      rec.p += m_displacement;
      rec.SetFaceNormal(movedRay, rec.n);
      return true;
   }

   bool BoundingBox(double time0, double time1, AABB& outputBox) const override
   {
      if (!m_src->BoundingBox(time0, time1, outputBox))
      {
         return false;
      }

      outputBox = AABB(outputBox.Minimum + m_displacement, outputBox.Maximum = m_displacement);
      return true;
   }

private:
   std::shared_ptr<Hittable> m_src;
   Vec3 m_displacement;

};

class RotateY : public Hittable
{
public:
   RotateY(std::shared_ptr<Hittable> src, double angleDegrees) :
      m_src(src)
   {
      double radians = DegreesToRadians(angleDegrees);
      m_sinTheta = std::sin(radians);
      m_cosTheta = std::cos(radians);
      m_bHasBox = src->BoundingBox(0.0, 1.0, m_boundingBox);

      Point3 min(Infinity, Infinity, Infinity);
      Point3 max(-Infinity, -Infinity, -Infinity);

      for (int dx = 0; dx < 2; ++dx)
      {
         for (int dy = 0; dy < 2; ++dy)
         {
            for (int dz = 0; dz < 2; ++dz)
            {
               double x = dx * m_boundingBox.Maximum.x + (1 - dx) * m_boundingBox.Minimum.x;
               double y = dy * m_boundingBox.Maximum.y + (1 - dy) * m_boundingBox.Minimum.y;
               double z = dz * m_boundingBox.Maximum.z + (1 - dz) * m_boundingBox.Minimum.z;

               auto newX = m_cosTheta * x + m_sinTheta * z;
               auto newZ = -m_sinTheta * x + m_cosTheta * z;

               Vec3 tester(newX, y, newZ);
               for (int component = 0; component < 3; ++component)
               {
                  min[component] = std::fmin(min[component], tester[component]);
                  max[component] = std::fmax(max[component], tester[component]);
               }
            }
         }
      }
   }

   bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
   {
      Point3 origin = r.Origin;
      Vec3 direction = r.Direction;

      origin[0] = m_cosTheta * r.Origin[0] - m_sinTheta * r.Origin[2];
      origin[2] = m_sinTheta * r.Origin[0] + m_cosTheta * r.Origin[2];

      direction[0] = m_cosTheta * r.Direction[0] - m_sinTheta * r.Direction[2];
      direction[2] = m_sinTheta * r.Direction[0] + m_cosTheta * r.Direction[2];

      Ray rotatedRay = Ray(origin, direction, r.Time);
      if (!m_src->Hit(rotatedRay, tMin, tMax, rec))
      {
         return false;
      }

      auto p = rec.p;
      auto normal = rec.n;

      p[0] = m_cosTheta * rec.p[0] + m_sinTheta * rec.p[2];
      p[2] = -m_sinTheta * rec.p[0] + m_cosTheta * rec.p[2];

      normal[0] = m_cosTheta * rec.n[0] + m_sinTheta * rec.n[2];
      normal[2] = -m_sinTheta * rec.n[0] + m_cosTheta * rec.n[2];

      rec.p = p;
      rec.SetFaceNormal(rotatedRay, normal);

      return true;
   }

   bool BoundingBox(double time0, double time1, AABB& outputBox) const override
   {
      outputBox = m_boundingBox;
      return m_bHasBox;
   }

private:
   std::shared_ptr<Hittable> m_src;
   double m_sinTheta;
   double m_cosTheta;
   bool m_bHasBox;
   AABB m_boundingBox;

};