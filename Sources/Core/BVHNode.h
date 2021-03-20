#pragma once
#include <Core/Hittable.h>
#include <Core/HittableList.h>

class BVHNode : public Hittable
{
public:
   BVHNode()
   {
   }

   BVHNode(const HittableList& list, double time0, double time1) :
      BVHNode(list.m_objects, 0, list.m_objects.size(), time0, time1)
   {
   }

   BVHNode(const std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0, double time1)
   {
      int axis = RandomInt(0, 2);
      auto boxComparator = [axis](const std::shared_ptr<Hittable> left, const std::shared_ptr<Hittable> right)->bool
      {
         AABB boxLeft;
         AABB boxRight;
         if (!left->BoundingBox(0.0, 0.0, boxLeft) || !right->BoundingBox(0.0, 0.0, boxRight))
         {
            std::cerr << "No bounding box in BVHNode constructor .\n";
         }

         return boxLeft.Minimum.e[axis] < boxRight.Minimum.e[axis];
      };

      size_t objectSpan = end - start;
      if (objectSpan == 1)
      {
         m_left = m_right = objects[start];
      }
      else if (objectSpan == 2)
      {
         if (boxComparator(objects[start], objects[start + 1]))
         {
            m_left = objects[start];
            m_right = objects[start + 1];
         }
         else
         {
            m_left = objects[start + 1];
            m_right = objects[start];
         }
      }
      else
      {
         std::sort(objects.begin() + start, objects.begin() + end, boxComparator);

         auto mid = start + objectSpan / 2;
         m_left = std::make_shared<BVHNode>(objects, start, mid, time0, time1);
         m_right = std::make_shared<BVHNode>(objects, mid, end, time0, time1);

         AABB boxLeft;
         AABB boxRight;
         if (!m_left->BoundingBox(time0, time1, boxLeft) || !m_right->BoundingBox(time0, time1, boxRight))
         {
            std::cerr << "No bounding box in BVHNode constructor! \n";
         }

         m_aabb = AABB::SurroundingBox(boxLeft, boxRight);
      }
   }

   bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
   {
      if (!m_aabb.Hit(r, tMin, tMax))
      {
         return false;
      }

      bool bHitLeft = m_left->Hit(r, tMin, tMax, rec);
      bool bHitRight = m_right->Hit(r, tMin, bHitLeft ? rec.t : tMax, rec);
      return bHitLeft || bHitRight;
   }

   bool BoundingBox(double time0, double time1, AABB& outputBox) const override
   {
      outputBox = m_aabb;
      return true;
   }

private:
   std::shared_ptr<Hittable> m_left;
   std::shared_ptr<Hittable> m_right;
   AABB m_aabb;

};