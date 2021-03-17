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

   virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
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

private:
   std::vector<std::shared_ptr<Hittable>> m_objects;

};
