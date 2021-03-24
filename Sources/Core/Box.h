#pragma once
#include <Core/Rect.h>
#include <Core/HittableList.h>

class Box : public Hittable
{
public:
   Box() = default;
   Box(const Point3& boxMin, const Point3& boxMax, std::shared_ptr<Material> matPtr) :
      m_boxMin(boxMin),
      m_boxMax(boxMax)
   {
      m_sides.Add(std::make_shared<XYRect>(boxMin.x, boxMax.x, boxMin.y, boxMax.y, boxMax.z, matPtr));
      m_sides.Add(std::make_shared<XYRect>(boxMin.x, boxMax.x, boxMin.y, boxMax.y, boxMin.z, matPtr));

      m_sides.Add(std::make_shared<XZRect>(boxMin.x, boxMax.x, boxMin.z, boxMax.z, boxMax.y, matPtr));
      m_sides.Add(std::make_shared<XZRect>(boxMin.x, boxMax.x, boxMin.z, boxMax.z, boxMin.y, matPtr));

      m_sides.Add(std::make_shared<YZRect>(boxMin.y, boxMax.y, boxMin.z, boxMax.z, boxMax.x, matPtr));
      m_sides.Add(std::make_shared<YZRect>(boxMin.y, boxMax.y, boxMin.z, boxMax.z, boxMin.x, matPtr));
   }

   bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
   {
      return m_sides.Hit(r, tMin, tMax, rec);
   }

   bool BoundingBox(double time0, double time1, AABB& outputBox) const override
   {
      outputBox = AABB(m_boxMin, m_boxMax);
      return true;
   }

private:
   Point3 m_boxMin = Point3(-0.5, -0.5, -0.5);
   Point3 m_boxMax = Point3(0.5, 0.5, 0.5);
   HittableList m_sides;

};
