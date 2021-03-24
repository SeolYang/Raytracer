#pragma once
#include <Core/Hittable.h>
#include <Core/Isotropic.h>

class ConstantMedium : public Hittable
{
public:
   ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> albedo) : 
      m_boundary(boundary),
      m_negInvDensity(-1.0/density),
      m_phaseFunction(std::make_shared<Isotropic>(albedo))
   {
   }

   ConstantMedium(std::shared_ptr<Hittable> boundary, double density, Color albedo) :
      ConstantMedium(boundary, density, std::make_shared<SolidColorTexture>(albedo))
   {
   }

   bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
   {
      constexpr bool bEnableDebug = false;
      const bool bDebugging = bEnableDebug && RandomDouble() <= 0.000001;

      HitRecord recs[2];
      if (!m_boundary->Hit(r, -Infinity, Infinity, recs[0]))
      {
         return false;
      }

      if (!m_boundary->Hit(r, recs[0].t + 0.0001, Infinity, recs[1]))
      {
         return false;
      }

      if (bDebugging)
      {
         std::cerr << "\ntMin = " << recs[0].t << ", tMax = " << recs[1].t << std::endl;
      }

      if (recs[0].t < tMin)
      {
         recs[0].t = tMin;
      }
      if (recs[1].t > tMax)
      {
         recs[1].t = tMax;
      }

      if (recs[0].t >= recs[1].t)
      {
         return false;
      }

      if (recs[0].t < 0.0)
      {
         recs[0].t = 0.0;
      }

      const auto rayLength = r.Direction.Length();
      const auto distanceInsideBoundary = (recs[1].t - recs[0].t) * rayLength;
      const auto hitDistance = m_negInvDensity * log(RandomDouble());

      if (hitDistance > distanceInsideBoundary)
      {
         return false;
      }

      rec.t = recs[0].t + (hitDistance / rayLength);
      rec.p = r.At(rec.t);
      if (bDebugging)
      {
         std::cerr << "Hit Distance = " << hitDistance << std::endl;
         std::cerr << "Rec.t = " << rec.t << std::endl;
         std::cerr << "Rec.p = " << rec.p << std::endl;
      }

      rec.n = Vec3(1.0, 0.0, 0.0);
      rec.bFrontFace = true;
      rec.MatPtr = m_phaseFunction;

      return true;
   }

   bool BoundingBox(double time0, double time1, AABB& outputBox) const override
   {
      return m_boundary->BoundingBox(time0, time1, outputBox);
   }

private:
   std::shared_ptr<Hittable> m_boundary;
   std::shared_ptr<Material> m_phaseFunction;
   double m_negInvDensity;

};

