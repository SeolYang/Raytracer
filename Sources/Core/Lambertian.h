#pragma once
#include <Core/CoreMinimal.h>
#include <Core/Hittable.h>
#include <Core/Material.h>

class Lambertian : public Material
{
public:
   Lambertian(const Color& albedo) :
      Albedo(albedo)
   {
   }

   bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
   {
      auto scatterDirection = RandomInHemisphere(rec.n);
      if (scatterDirection.IsNearZero())
      {
         scatterDirection = rec.n;
      }

      scattered = Ray(rec.p, scatterDirection);
      attenuation = Albedo;
      return true;
   }

public:
   Color Albedo;

};
