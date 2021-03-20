#pragma once
#include <Core/Hittable.h>

class HittableList : public Hittable
{
public:
   HittableList()
   {
   }

   HittableList(std::shared_ptr<Hittable> object)
   {
   }

   void Clear()
   {
      m_objects.clear();
   }

   void Add(std::shared_ptr<Hittable> object)
   {
      m_objects.push_back(object);
   }

   std::vector<std::shared_ptr<Hittable>>& GetObjects() { return m_objects; }
   const std::vector<std::shared_ptr<Hittable>>& GetObjects() const { return m_objects; }

   bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
   {
      HitRecord tempRec;
      bool bHitAnything = false;
      auto closestSoFar = tMax;

      for (const auto& object : m_objects)
      {
         if (object->Hit(r, tMin, closestSoFar, tempRec))
         {
            bHitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
         }
      }

      return bHitAnything;
   }

   bool BoundingBox(double time0, double time1, AABB& outputBox) const override
   {
      if (m_objects.empty())
      {
         return false;
      }

      AABB tempBox;
      bool bFirstBox = true;

      for (const auto& object : m_objects)
      {
         if (!object->BoundingBox(time0, time1, tempBox))
         {
            outputBox = bFirstBox ? tempBox : AABB::SurroundingBox(tempBox, outputBox);
            bFirstBox = false;
         }
      }

      return true;
   }

private:
   friend class BVHNode;
   std::vector<std::shared_ptr<Hittable>> m_objects;

};
