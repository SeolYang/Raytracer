#pragma once
#include <Math/Ray.h>
#include <Core/Hittable.h>
#include <Core/Material.h>
#include <Core/Texture.h>

class Isotropic : public Material
{
public:
   Isotropic(std::shared_ptr<Texture> albedo) :
      m_albedo(albedo)
   {
   }

   Isotropic(Color albedo) :
      m_albedo(std::make_shared<SolidColorTexture>(albedo))
   {
   }

   bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
   {
      scattered = Ray(rec.p, RandomInUnitSphere(), rayIn.Time);
      attenuation = m_albedo->Value(rec.u, rec.v, rec.p);
      return true;
   }

private:
   std::shared_ptr<Texture> m_albedo;
};
