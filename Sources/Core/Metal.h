#pragma once
#include <Core/CoreMinimal.h>
#include <Core/Material.h>
#include <Core/Hittable.h>
#include <Math/Ray.h>

class Metal : public Material
{
public:
   Metal(const Color& albedo, double fuzz/* Roughness */) :
      Albedo(albedo),
      Fuzz(fuzz < 1.0 ? fuzz : 1.0)
   {
   }

   bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
   {
      Vec3 reflected = Reflect(UnitVectorOf(rayIn.Direction), rec.n);
      scattered = Ray(rec.p, reflected + Fuzz*RandomInHemisphere(rec.n));
      attenuation = Albedo;
      return (Dot(scattered.Direction, rec.n) > 0.0);
   }

public:
   Color Albedo;
   double Fuzz;

};
