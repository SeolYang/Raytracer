#pragma once
#include <Core/CoreMinimal.h>
#include <Core/Hittable.h>
#include <Core/Material.h>
#include <Math/Ray.h>

class Dielectric : public Material
{
public:
   Dielectric(double ior) :
      IOR(ior)
   {
   }

   bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
   {
      attenuation = Color(1.0, 1.0, 1.0);
      double refractionRatio = rec.bFrontFace ? (1.0 / IOR) : IOR;

      Vec3 unitDir = UnitVectorOf(rayIn.Direction);
      double cosTheta = std::fmin(Dot(-unitDir, rec.n), 1.0);
      double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

      bool bCannotRefract = refractionRatio * sinTheta > 1.0;
      Vec3 dir;
      if (bCannotRefract || Reflectance(cosTheta, refractionRatio) > RandomDouble())
      {
         dir = Reflect(unitDir, rec.n);
      }
      else
      {
         dir = Refract(unitDir, rec.n, refractionRatio);
      }


      scattered = Ray(rec.p, dir, rayIn.Time);
      return true;
   }

private:
   static double Reflectance(double cosine, double refIdx)
   {
      // Schlick's approximation for reflectance
      auto r0 = (1.0 - refIdx) / (1 + refIdx);
      r0 = r0 * r0;
      return r0 + (1.0 - r0) * pow((1.0 - cosine), 5);
   }

public:
   double IOR;

};
